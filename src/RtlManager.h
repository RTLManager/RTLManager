#pragma once
#include <map>
//#include <winnt.h>
//#include <minwindef.h>
#include "Headers/PluginInterface.h"

#include <vector>



using namespace std;







bool isCurrentRTL();
void changeTextDirection(bool isRtl);
void bufferChanged();
bool isFirstCharRTL();
void toggleRtl();

void fileBeforeClose();
HWND getLastView();
HWND getCurrentView();
