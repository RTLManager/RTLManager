#pragma once
#include <map>
//#include <winnt.h>
//#include <minwindef.h>
#include "Headers/PluginInterface.h"
#include <ctime>
#include <vector>

struct StampedBool {
	bool isRtl;
	time_t lastUpdate;
};

using namespace std;







bool isCurrentRTL();
void changeTextDirection(bool isRtl);
void bufferChanged();
bool isFirstCharRTL();
void toggleRtl();

void fileBeforeClose();
HWND getLastView();
HWND getCurrentView();

