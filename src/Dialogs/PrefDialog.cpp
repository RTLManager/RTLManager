/*
This file is part of Explorer Plugin for Notepad++
Copyright (C)2006 Jens Lorenz <jens.plugin.npp@gmx.de>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "PrefDialog.h"
#include "../Headers/NativeLang_def.h"
#include "resource.h"
#include <string>

using namespace std;

extern DefaultPref defaultPref;

void PrefDialog::doDialog()
{
	if (!isCreated())
		create(IDD_PREF_DIALOG);

	goToCenter();
}


INT_PTR CALLBACK PrefDialog::run_dlgProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	LRESULT dpSelection;
	switch (Message)
	{
	case WM_INITDIALOG:
	{

		/* Change dialog lang */
		NLChangeDialog(_hInst, _nppData._nppHandle, _hSelf, _T("Preferences"));
		::SendDlgItemMessage(_hSelf, IDD_COMBOBOX_DEFAULT_ACTION, CB_ADDSTRING, 0, (LPARAM)TEXT("According to first character (Experimental)"));
		::SendDlgItemMessage(_hSelf, IDD_COMBOBOX_DEFAULT_ACTION, CB_ADDSTRING, 0, (LPARAM)TEXT("RTL"));
		::SendDlgItemMessage(_hSelf, IDD_COMBOBOX_DEFAULT_ACTION, CB_ADDSTRING, 0, (LPARAM)TEXT("LTR"));
		::SendDlgItemMessage(_hSelf, IDD_COMBOBOX_DEFAULT_ACTION, CB_ADDSTRING, 0, (LPARAM)TEXT("Don't do anything"));
		//int sel = defaultPref;
		::SendDlgItemMessage(_hSelf, IDD_COMBOBOX_DEFAULT_ACTION, CB_SETCURSEL, defaultPref, 0);



		return TRUE;
	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case IDCANCEL:
			display(FALSE);
			return TRUE;
			break;
		case IDOK:
			dpSelection = ::SendDlgItemMessage(_hSelf, IDD_COMBOBOX_DEFAULT_ACTION, CB_GETCURSEL, 0, 0);
			if (dpSelection != CB_ERR) {
				defaultPref = (DefaultPref)dpSelection;
			}
			display(FALSE);
			return TRUE;
			break;
		default:
			break;
		}
	}
	}
	return FALSE;
}

