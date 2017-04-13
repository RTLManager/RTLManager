#pragma once
#include <map>
//#include <winnt.h>
//#include <minwindef.h>
#include "Headers/PluginInterface.h"

#include <vector>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>



using namespace std;

extern NppData nppData;



//a comparator to compare between 2 TCHAR pointers
//for the fileMap 
struct TcharVectorComp {


	bool operator()(const vector<TCHAR> a, const vector<TCHAR> b) const {
		size_t i;
		for (i = 0; i < a.size() && i < b.size(); i++) {
			if (a[i] == '\0') {
				return !(b[i] == '\0');
			}
			if (b[i] == '\0') {
				return false;
			}
			if (a[i] != b[i]) {
				return (a[i] > b[i]);
			}
		}
		return i >= a.size() && !(i >= b.size());
	}


};

void saveSettings();
void loadSettings();
bool isCurrentRTL();
void changeTextDirection(bool isRtl);
void bufferChanged();
bool isFirstCharRTL();
void toggleRtl();

void fileBeforeClose();
HWND getLastView();
HWND getCurrentView();
