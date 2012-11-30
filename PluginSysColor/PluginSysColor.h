/*
  Copyright (C) 2012 Brian Ferguson

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

#include "StdAfx.h"

// Following enumerated types corespond directly to these values: http://msdn.microsoft.com/en-us/library/windows/desktop/ms724371%28v=vs.85%29.aspx
//	 (except Glass which is retrieved through DWM)
enum MeasureType
{
	SYSCOLOR_SCROLLBAR = 0,
	SYSCOLOR_DESKTOP,
	SYSCOLOR_ACTIVECAPTION,
	SYSCOLOR_INACTIVECAPTION,
	SYSCOLOR_MENU,
	SYSCOLOR_WINDOW,
	SYSCOLOR_WINDOWFRAME,
	SYSCOLOR_MENUTEXT,
	SYSCOLOR_WINDOWTEXT,
	SYSCOLOR_CAPTIONTEXT,
	SYSCOLOR_ACTIVEBORDER,
	SYSCOLOR_INACTIVEBORDER,
	SYSCOLOR_APPWORKSPACE,
	SYSCOLOR_HIGHLIGHT,
	SYSCOLOR_HIGHLIGHTTEXT,
	SYSCOLOR_BUTTONFACE,
	SYSCOLOR_BUTTONSHADOW,
	SYSCOLOR_GRAYTEXT,
	SYSCOLOR_BUTTONTEXT,
	SYSCOLOR_INACTIVECAPTIONTEXT,
	SYSCOLOR_BUTTONHIGHLIGHT,
	SYSCOLOR_3DDARKSHADOW,
	SYSCOLOR_3DLIGHT,
	SYSCOLOR_INFOTEXT,
	SYSCOLOR_INFOBACKGROUND,
	SYSCOLOR_GLASS,							// Special case for current aero color (25 is not defined)
	SYSCOLOR_HOTLIGHT,
	SYSCOLOR_ACTIVECAPTIONGRADIENT,
	SYSCOLOR_INACTIVECAPTIONGRADIENT,
	SYSCOLOR_MENUHIGHLIGHT,
	SYSCOLOR_MENUBAR
};

enum DisplayType
{
	DISPLAY_ALL,
	DISPLAY_RED,
	DISPLAY_GREEN,
	DISPLAY_BLUE,
	DISPLAY_ALPHA,
	DISPLAY_RGB			// Displays only red, green, blue values (No alpha)
};

struct Measure
{
	std::wstring color;
	bool isHex;
	bool isXP;

	MeasureType colorType;
	DisplayType displayType;

	Measure() : 
		color(L""),
		isHex(false),
		isXP(false),
		colorType(SYSCOLOR_DESKTOP),
		displayType(DISPLAY_ALL)
	{
	}
};