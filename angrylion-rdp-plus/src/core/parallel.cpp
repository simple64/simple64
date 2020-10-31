#include "parallel.h"

#include <atomic>
#include <algorithm>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

class Parallel
{
public:
    Parallel(std::uint32_t num_workers)
    {
        if (num_workers == 0) {
            // auto-select number of workers based on the number of cores
            m_num_workers = std::thread::hardware_concurrency();
        } else {
            m_num_workers = std::min(num_workers, PARALLEL_MAX_WORKERS);
        }

        // mask for m_tasks_done when all workers have finished their task
        if (m_num_workers == PARALLEL_MAX_WORKERS) {
            m_all_tasks_done = ~0ULL;
        } else {
            m_all_tasks_done = (1ULL << m_num_workers) - 1;
        }

        // except worker 0, which runs in the main thread
        m_all_tasks_done--;

        // give workers an empty task
        m_task = [](std::uint32_t) {};
        m_accept_work = true;
        start_work();

        // create worker threads
        for (std::uint32_t worker_id = 1; worker_id < m_num_workers; worker_id++) {
            m_workers.emplace_back(std::thread(&Parallel::do_work, this, worker_id));
        }

        // synchronize workers to prepare them for real tasks
        wait();
    }

    ~Parallel() {
        // wait for all workers to finish their current work
        wait();

        // exit worker main loops
        m_accept_work = false;
        start_work();

        // join worker threads to make sure they have finished
        for (auto& thread : m_workers) {
            thread.join();
        }

        // destroy all worker threads
        m_workers.clear();
    }

    void run(std::function<void(std::uint32_t)>&& task) {
        // don't allow more tasks if workers are stopping
        if (!m_accept_work) {
            throw std::runtime_error("Workers are exiting and no longer accept work");
        }

        // prepare task for workers and send signal so they start working
        m_task = std::move(task);
        start_work();

        // run worker 0 directly on main thread
        m_task(0);

        // wait for all workers to finish
        wait();
    }

    std::uint32_t num_workers() {
        return m_num_workers;
    }

private:
    std::function<void(std::uint32_t)> m_task;
    std::vector<std::thread> m_workers;
    std::mutex m_signal_mutex;
    std::condition_variable m_signal_work;
    std::condition_variable m_signal_done;
    std::atomic<uint64_t> m_tasks_done;
    std::uint64_t m_all_tasks_done;
    std::atomic<bool> m_accept_work;
    std::uint32_t m_num_workers;

    void start_work() {
        std::unique_lock<std::mutex> ul(m_signal_mutex);

        // clear task bits for all workers
        m_tasks_done = 0;

        // wake up all workers
        m_signal_work.notify_all();
    }

    void do_work(std::uint32_t worker_id) {
        const std::uint64_t worker_mask = 1LL << worker_id;

        while (m_accept_work) {
            // do the work
            m_task(worker_id);

            {
                std::unique_lock<std::mutex> ul(m_signal_mutex);

                // mark task as done
                m_tasks_done |= worker_mask;

                // notify main thread
                m_signal_done.notify_one();
#if !defined(ANDROID)
                // take a break and wait for more work
                m_signal_work.wait(ul, [worker_mask, this] {
                    return (m_tasks_done & worker_mask) == 0;
                });
            }
#else
            //Busy loop wait for a bit to keep cores awake
            bool workAvailable = false;
            int count = 0;
            while (!workAvailable && count++ < 10000) {
                std::unique_lock<std::mutex> ul(m_signal_mutex);
                workAvailable = (m_tasks_done & worker_mask) == 0;
                std::this_thread::yield();
            }

            std::unique_lock<std::mutex> ul(m_signal_mutex);

            // take a break and wait for more work
            m_signal_work.wait(ul, [worker_mask, this] {
                return (m_tasks_done & worker_mask) == 0;
            });
#endif
        }
    }

    void wait() {

#ifdef ANDROID
        //Busy loop wait for a bit to keep cores awake
        bool waitingForWorkDone = true;
        int count = 0;
        while (waitingForWorkDone && count++ < 10000) {
            waitingForWorkDone = m_tasks_done != m_all_tasks_done;
            std::this_thread::yield();
        }
#endif

        // wait for all workers to set their task bits
        std::unique_lock<std::mutex> ul(m_signal_mutex);
        m_signal_done.wait(ul, [this] {
            return m_tasks_done == m_all_tasks_done;
        });
    }

    void operator=(const Parallel&) = delete;
    Parallel(const Parallel&) = delete;
};

// C interface for the Parallel class
static std::unique_ptr<Parallel> parallel;

void parallel_init(uint32_t num)
{
    parallel = std::make_unique<Parallel>(num);
}

void parallel_run(void task(uint32_t))
{
    parallel->run(task);
}

uint32_t parallel_num_workers()
{
    return parallel->num_workers();
}

void parallel_close()
{
    parallel.reset();
}
