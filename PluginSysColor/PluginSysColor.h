/*
  Copyright (C) 2014 Brian Ferguson

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

typedef struct COLORIZATIONPARAMS
{
	COLORREF	colorizationColor;
	COLORREF	colorizationAfterglow;
	UINT		colorizationColorBalance;
	UINT		colorizationAfterglowBalance;
	UINT		colorizationBlurBalance;
	UINT		colorizationGlassReflectionIntensity;
	BOOL		colorizationOpaqueBlend;
} DWMColorizationParameters;

typedef HRESULT(WINAPI * FPDWMGETCOLORIZATIONPARAMETERS)(COLORIZATIONPARAMS* pColorParams);
static FPDWMGETCOLORIZATIONPARAMETERS c_DwmGetColorizationParameters = nullptr;

typedef HRESULT(WINAPI * FPDWMISCOMPOSITIONENABLED)(BOOL *pfEnabled);
static FPDWMISCOMPOSITIONENABLED c_DwmIsCompositionEnabled = nullptr;

typedef HRESULT (WINAPI * FPDWMGETCOLORIZATIONCOLOR)(DWORD* pcrColorization, BOOL* pfOpaqueBlend);
static FPDWMGETCOLORIZATIONCOLOR c_DwmGetColorizationColor = nullptr;

// Following enumerated types corespond directly to these values: http://msdn.microsoft.com/en-us/library/windows/desktop/ms724371%28v=vs.85%29.aspx
// (except for the Windows 7 & 8 options - which is retrieved through DWM)
enum ColorType
{
	SCROLLBAR = 0,
	DESKTOP,
	ACTIVECAPTION,
	INACTIVECAPTION,
	MENU,
	WINDOW,
	WINDOWFRAME,
	MENUTEXT,
	WINDOWTEXT,
	CAPTIONTEXT,
	ACTIVEBORDER,
	INACTIVEBORDER,
	APPWORKSPACE,
	HIGHLIGHT,
	HIGHLIGHTTEXT,
	BUTTONFACE,
	BUTTONSHADOW,
	GRAYTEXT,
	BUTTONTEXT,
	INACTIVECAPTIONTEXT,
	BUTTONHIGHLIGHT,
	DDARKSHADOW,				// 3DDarkShadow
	DLIGHT,						// 3DLight
	INFOTEXT,
	INFOBACKGROUND,
	NOTHING,					// 25 is undefined
	HOTLIGHT,
	ACTIVECAPTIONGRADIENT,
	INACTIVECAPTIONGRADIENT,
	MENUHIGHLIGHT,
	MENUBAR,
	/*...*/
	WIN7_AERO,					// RGBA returned by DwmGetColorizationColor function http://msdn.microsoft.com/en-us/library/windows/desktop/aa969513%28v=vs.85%29.aspx
	WIN8_WINDOW,				// Following types are retreived by undocumented function "DwmGetColorizationParameters"
	DWM_COLORIZATION_COLOR,
	DWM_AFTERGLOW_COLOR,
	DWM_COLOR_BALANCE,
	DWM_AFTERGLOW_BALANCE,
	DWM_BLUR_BALANCE,
	DWM_GLASS_REFLECTION_INTENSITY,
	DWM_OPAQUE_BLEND
};

enum DisplayType
{
	ALL,
	RED,
	GREEN,
	BLUE,
	ALPHA,
	RGB							// Displays only red, green, blue values (No alpha)
};

struct Measure
{
	std::wstring color;
	bool isHex;
	bool isXP;

	ColorType colorType;
	DisplayType displayType;

	void* rm;

	Measure() : 
		color(L""),
		isHex(false),
		isXP(false),
		colorType(DESKTOP),
		displayType(ALL),
		rm()
	{ }
};
