#include <QObject>
#include <stdio.h>
#include "FullscreenResolutions.h"
#include "../Config.h"
#include "../mupenplus/GLideN64_mupenplus.h"

static struct
{
	struct
	{
		uint32_t width, height, refreshRate;
	} selected;

	struct
	{
		uint32_t width, height;
	} resolution[32];

	uint32_t refreshRate[32];

	uint32_t numResolutions;
	uint32_t numRefreshRates;
} fullscreen;

static void _fillFullscreenRefreshRateList(QStringList &_listRefreshRates, int &_rateIdx)
{
	memset(&fullscreen.refreshRate, 0, sizeof(fullscreen.refreshRate));
	fullscreen.numRefreshRates = 0;
	_rateIdx = 0;

	m64p_error ret;
	m64p_2d_size size;
	size.uiHeight = fullscreen.selected.height;
	size.uiWidth = fullscreen.selected.width;

	int num_rates = 32;
	int *rates = (int *)malloc(num_rates * sizeof(int));

	ret = CoreVideo_ListFullscreenRates(size, &num_rates, rates);

	if (ret != M64ERR_SUCCESS)
		return;

	for (int i = 0; i < num_rates; i++)
	{
		int refreshRate = rates[i];

		_listRefreshRates.append(QString::number(refreshRate) + " Hz");
		fullscreen.refreshRate[fullscreen.numRefreshRates] = refreshRate;

		if (fullscreen.selected.refreshRate == refreshRate)
			_rateIdx = fullscreen.numRefreshRates;

		fullscreen.numRefreshRates++;
	}

	free(rates);
}

void fillFullscreenResolutionsList(QStringList &_listResolutions, int &_resolutionIdx, QStringList &_listRefreshRates, int &_rateIdx)
{
	fullscreen.selected.width = config.video.fullscreenWidth;
	fullscreen.selected.height = config.video.fullscreenHeight;
	fullscreen.selected.refreshRate = config.video.fullscreenRefresh;

	memset(&fullscreen.resolution, 0, sizeof(fullscreen.resolution));
	memset(&fullscreen.refreshRate, 0, sizeof(fullscreen.refreshRate));
	fullscreen.numResolutions = 0;
	fullscreen.numRefreshRates = 0;
	_resolutionIdx = 0;

	int resolutions_length = 32;
	m64p_2d_size *resolutions = (m64p_2d_size *)malloc(resolutions_length * sizeof(m64p_2d_size));
	m64p_error ret;

	ret = CoreVideo_ListFullscreenModes(resolutions, &resolutions_length);

	if (ret != M64ERR_SUCCESS)
		return;

	for (int i = 0; i < resolutions_length; i++)
	{
		fullscreen.resolution[fullscreen.numResolutions].width = resolutions[i].uiWidth;
		fullscreen.resolution[fullscreen.numResolutions].height = resolutions[i].uiHeight;

		_listResolutions.append(QString::number(resolutions[i].uiWidth) + " x " + QString::number(resolutions[i].uiHeight));

		if ((fullscreen.selected.width == resolutions[i].uiWidth) &&
			(fullscreen.selected.height == resolutions[i].uiHeight))
			_resolutionIdx = fullscreen.numResolutions;

		fullscreen.numResolutions++;
	}

	_fillFullscreenRefreshRateList(_listRefreshRates, _rateIdx);

	free(resolutions);
}

void fillFullscreenRefreshRateList(int _resolutionIdx, QStringList &_listRefreshRates, int &_rateIdx)
{
	fullscreen.selected.width = fullscreen.resolution[_resolutionIdx].width;
	fullscreen.selected.height = fullscreen.resolution[_resolutionIdx].height;
	_fillFullscreenRefreshRateList(_listRefreshRates, _rateIdx);
	_rateIdx = fullscreen.numRefreshRates - 1;
}

void getFullscreenResolutions(int _idx, unsigned int &_width, unsigned int &_height)
{
	_width = fullscreen.resolution[_idx].width;
	_height = fullscreen.resolution[_idx].height;
}

void getFullscreenRefreshRate(int _idx, unsigned int &_rate)
{
	_rate = fullscreen.refreshRate[_idx];
}
