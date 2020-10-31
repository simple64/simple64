#pragma once

#include <vector>
#include <stdint.h>

namespace RSP
{
namespace JIT
{
class Allocator
{
public:
	Allocator() = default;
	~Allocator();
	void operator=(const Allocator &) = delete;
	Allocator(const Allocator &) = delete;

	void *allocate_code(size_t size);
	static bool commit_code(void *code, size_t size);

private:
	struct Block
	{
		uint8_t *code = nullptr;
		size_t size = 0;
		size_t offset = 0;
	};
	std::vector<Block> blocks;

	static Block reserve_block(size_t size);
};
}
}
