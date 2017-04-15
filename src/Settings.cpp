#include "Settings.h"
#include <fstream>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include "Headers\Notepad_plus_msgs.h"
#include "Headers\PluginInterface.h"


TCHAR iniFilePath[MAX_PATH];
const TCHAR configFileName[] = TEXT("RtlManager.ini");

const string FLAG = "V0.8OK";
#pragma section("SHARED", read, write, shared) 

__declspec(allocate("SHARED")) DefaultPref defaultPref;

extern bool isRtl;
extern NppData nppData;
extern map<vector<TCHAR>, char, TcharVectorComp> fileMap;




  
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

	std::ofstream iniFile;

	iniFile.open(iniFilePath, ios::binary | ios::trunc);

	if (!iniFile.is_open()) {
		return;
	}
	boost::archive::binary_oarchive boa(iniFile);
	//marking the file

	boa << FLAG;
	boa << defaultPref;
	boa << isRtl;
	boa << fileMap;
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
		return;
	}
	ia >> defaultPref;
	ia >> isRtl;
	ia >> fileMap;

	iniFile.close();
}