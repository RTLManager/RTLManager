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


#ifndef PREF_DEFINE_H
#define PREF_DEFINE_H

#include "StaticDialog.h"
#include "resource.h"
#include "../Headers/PluginInterface.h"
#include "URLCtrl.h"


enum DefaultPref {
	FirstChar = 0,
	RTL = 1,
	LTR = 2,
	Nothing = 3
};


class PrefDialog : public StaticDialog
{

public:
	PrefDialog() : StaticDialog() {};

	void init(HINSTANCE hInst, NppData nppData)
	{
		_nppData = nppData;
		Window::init(hInst, nppData._nppHandle);
	};

	void doDialog();



protected:
	virtual BOOL  CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	/* Handles */
	NppData			_nppData;
	HWND			_HSource;
};



#endif // HELP_DEFINE_H
