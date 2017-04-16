#include "Settings.h"
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include "Headers\Notepad_plus_msgs.h"
#include "Headers\PluginInterface.h"
#include <vector>


TCHAR iniFilePath[MAX_PATH];
const TCHAR configFileName[] = TEXT("RtlManager.ini");

const string FLAG = "V0.9OK";
const string V8FLAG = "V0.8OK";
#pragma section("SHARED", read, write, shared) 

__declspec(allocate("SHARED")) DefaultPref defaultPref;

struct StampedBool {
	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & isRtl;
		ar & lastUpdate;
	}
	bool isRtl;
	time_t lastUpdate;
};

extern bool isRtl;
extern NppData nppData;
extern map<vector<TCHAR>, StampedBool, TcharVectorComp> fileMap;
int maxEntries;



  
void buildIniPath()
{
	::SendMessage(nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)iniFilePath);

	// if config path doesn't exist, we create it
	if (PathFileExists(iniFilePath) == FALSE)
	{
		::CreateDirectory(iniFilePath, NULL);
	}

	// make your plugin config file full file path name
	PathAppend(iniFilePath, configFileName);

}

__declspec(allocate("SHARED")) bool isLoaded = false;
bool isFirstLoad;

void saveSettings() {
	if (!isFirstLoad) {
		return;
	}

	//in order to avoid running this function every time an entry is added
	//will wait till it passes 20% of the max entries
	//TODO let the user customize the ceiling that will trigger the function
	if (fileMap.size() > maxEntries * 1.2) {
		reduceFileMap(maxEntries);
	}

	std::ofstream iniFile;

	iniFile.open(iniFilePath, ios::binary | ios::trunc);

	if (!iniFile.is_open()) {
		return;
	}
	boost::archive::binary_oarchive boa(iniFile);
	//marking the file

	boa << FLAG;
	boa << fileMap;
	boa << defaultPref;
	boa << isRtl;
	boa << maxEntries;
	iniFile.close();
}

//loads settings from ini file
void loadSettings() {

	isFirstLoad = !isLoaded;
	isLoaded = true;
	if (!isFirstLoad) {
		return;
	}
	std::ifstream iniFile(iniFilePath, ios::binary);
	if (!iniFile.is_open()) {
		return;
	}
	boost::archive::binary_iarchive ia(iniFile);
	// read class state from archive
	string flag;
	ia >> flag;
	//checking if the settings file is OK
	//returning if it's not
	if (flag.compare(FLAG) != 0) {
		if (flag.compare(V8FLAG) == 0) {
			v8LoadSettings(ia);
		}
		return;
	}
	ia >> fileMap;
	ia >> defaultPref;
	ia >> isRtl;
	ia >> maxEntries;
	iniFile.close();
}

//pair<vector<TCHAR>, StampedBool> unaryOp(pair<vector<TCHAR>, char> src) {
//	StampedBool temp;
//	temp.isRtl = src.second;
//	temp.lastUpdate = time(NULL);
//	return pair<vector<TCHAR>, StampedBool>(src.first, temp);
//	
//}

///reverse compatibility for v8 where the map had a different structure
/// the function loads the settings files and converts the old map to the current map structure
void v8LoadSettings(boost::archive::binary_iarchive &ia ) {
	map<vector<TCHAR>, char> tempMap;
	ia >> defaultPref;
	ia >> isRtl;
	ia >> tempMap;
	maxEntries = 1000;
	for (map<vector<TCHAR>, char>::iterator it = tempMap.begin(); it != tempMap.end(); it++) {
		StampedBool tempSB;
		tempSB.lastUpdate = time(NULL);
		tempSB.isRtl = it->second;
		fileMap.insert(pair<vector<TCHAR>, StampedBool>(it->first, tempSB));
	}
	/*
	std transform gives the followin error:
	Severity	Code	Description	Project	File	Line	Suppression State
	Error	C2678	binary '=': no operator found which takes a left-hand operand of type 'const std::vector<TCHAR,std::allocator<wchar_t>>' (or there is no acceptable conversion)	RTLManager	f:\vs17\vc\tools\msvc\14.10.25017\include\utility	238	
	*/
	//std::transform(temp.begin(), temp.end(), fileMap.begin(), unaryOp);
}

// a comperison for the std::sort method
//the order will be a decending order based on 'last update'
bool sortComp(pair<vector<TCHAR>, StampedBool> a, pair<vector<TCHAR>, StampedBool> b) {
	return a.second.lastUpdate > b.second.lastUpdate;
}

/// reduces the size of the file map (up) to the size of max
/// the elements that will be removed will be the oldest (according to 'last update' field) elements in the map
void reduceFileMap(int max) {
	if (fileMap.size() <= max) {
		return;
	}
	vector<pair<vector<TCHAR>, StampedBool>> vec(fileMap.begin(), fileMap.end());
	std::sort(vec.begin(), vec.end(), sortComp);
	fileMap.clear();
	fileMap.insert(vec.begin(), vec.begin() + max);
}