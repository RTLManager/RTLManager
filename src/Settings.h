#pragma once
#include <iostream>
#include <shlwapi.h>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

using namespace std;


enum DefaultPref {
	FirstChar = 0,
	RTL = 1,
	LTR = 2,
	Nothing = 3
};

void buildIniPath();
void saveSettings();
void loadSettings();
//a comparator to compare between 2 TCHAR pointers
//for the fileMap 
struct TcharVectorComp {


	bool operator()(
		const vector<TCHAR> a, const vector<TCHAR> b
		) const {
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

void v8LoadSettings(boost::archive::binary_iarchive &ia );

