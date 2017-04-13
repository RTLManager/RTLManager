
#include "RtlManager.h"
#include "PluginDefinition.h"
#include <fstream>
#include <ctime>
#include "Dialogs\PrefDialog.h"
#include "Headers\Notepad_plus_msgs.h"
#include "Headers\menuCmdID.h"

//a map from the file paths to a 
//bool that states if the file should be in rtl
map<vector<TCHAR>, char, TcharVectorComp> fileMap;
extern TCHAR iniFilePath[MAX_PATH];
const string FLAG = "V0.8OK";
extern DefaultPref defaultPref;
bool isRtl;
HWND lastView;



//represents the current buffer (tab) full path
vector<TCHAR> currentTabPath;
int maxSize = 1000;

// saves the settings to the ini file
void saveSettings() {
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

//checks if the current buffer (tab) in NPP is in rtl mode
//RETURN VALUE: true if it's in rtl, false otherwise
bool isCurrentRTL() {
	long exStyle = static_cast<long>(::GetWindowLongPtr(getLastView(), GWL_EXSTYLE));
	return (exStyle & WS_EX_LAYOUTRTL) != 0;
}

//saves the last view and returns it the next time the function will be called
//If there's no last view saved here the function returns the current view
//thanks to Yaron for the reference and pnedev for this function
 HWND getLastView()
{
	 HWND temp = lastView;
	 lastView = getCurrentView();
	 return (temp != NULL)? temp : lastView;
}

//returns the current view 
HWND getCurrentView(){
	return (::SendMessage(nppData._nppHandle, NPPMSG + 88, 0, 0) == MAIN_VIEW) ?
		nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
}

//changes text direction of the current buffer (tab) in NPP to rtl or ltr, depends on the toRtl parameter
void changeTextDirection(bool toRtl) {
	::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, (toRtl ? IDM_EDIT_RTL : IDM_EDIT_LTR));
	isRtl = toRtl;
}

//toggle between rtl and ltr, 
void toggleRtl() {
	changeTextDirection(!isRtl);
}

//checks if the first char in the current open document is from an rtl language (Hebrew, Arabic - 0xD7/8/9)
bool isFirstCharRTL() {
	unsigned char ch =  ::SendMessage(getCurrentView(), SCI_GETCHARAT, 0, 0);
	return ch == 0xD7 || ch == 0xD8 || ch == 0xD9 || ch == 0xEF || ch ==0xDD || ch == 0xDA;
}


//checks the length of a TCHAR string, assuming that
//it's a path and the max length is MAX_PATH
int getTcharPathLength(TCHAR * path) {
	if (path == NULL) {
		return -1;
	}
	for (int i = 0; i < MAX_PATH; i++) {
		if (path[i] == '\0') {
			return i;
		}
	}
	return MAX_PATH;

}

//runs on buffer changed event, 
//saves the old tab settings and loads the new tab's settings (if exists)
void bufferChanged() {
	TCHAR* newPath = new TCHAR[MAX_PATH];
	::SendMessage(nppData._nppHandle, NPPM_GETFULLCURRENTPATH, 0, (LPARAM)newPath);
	
	if (!currentTabPath.empty()) {
		fileMap.insert_or_assign(currentTabPath, isCurrentRTL());
	}
	currentTabPath.clear();
	currentTabPath.insert(currentTabPath.begin(), newPath, newPath + getTcharPathLength(newPath));
	delete[] newPath;
	map<vector<TCHAR>, char, vector<TCHAR>>::iterator it = fileMap.find(currentTabPath);

	if (it != fileMap.end()) {
		changeTextDirection(it->second);
	}
	else {
		switch (defaultPref) {
		case FirstChar:
			changeTextDirection(isFirstCharRTL());
			break;
		case RTL:
			changeTextDirection(true);
			break;
		case LTR:
			changeTextDirection(false);
		}
	}
}

//void bufferChangedStart()
//{
//	HANDLE hThread = ::CreateThread(NULL, 0, bufferChanged, 0, 0, NULL);
//	::CloseHandle(hThread);
//}

//thie runs before a file is closed, if the file is labled with 'new' then
//the associated record will be deleted from fileMap
void fileBeforeClose() {
	if (currentTabPath.empty()) {
		return;
	}
	if (currentTabPath[0] == 'n' && currentTabPath[1] == 'e' && currentTabPath[2] == 'w') {
		fileMap.erase(currentTabPath);
		currentTabPath.clear();
	}
}

