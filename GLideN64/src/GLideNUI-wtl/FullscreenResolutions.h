#pragma once
#include <vector>
#include <string>

typedef std::vector<std::string> StringList;

void fillFullscreenResolutionsList(StringList & _listResolutions, int & _resolutionIdx, StringList & _listRefreshRates, int & _rateIdx);
void fillFullscreenRefreshRateList(int _resolutionIdx, StringList & _listRefreshRates, int & _rateIdx);
void getFullscreenResolutions(int _idx, unsigned int & _width, unsigned int & _height);
void getFullscreenRefreshRate(int _idx, unsigned int & _rate);