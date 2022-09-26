/* Copyright (C) 2022 Brian Ferguson
 *
 * This Source Code Form is subject to the terms of the GNU General Public
 * License; either version 2 of the License, or (at your option) any later
 * version. If a copy of the GPL was not distributed with this file, You can
 * obtain one at <https://www.gnu.org/licenses/gpl-2.0.html>. */

#include <Windows.h>
#include <dwmapi.h>
#include <process.h>
#include <Uxtheme.h>
#include <VersionHelpers.h>
#include <wininet.h>
#include <string>
#include <vector>
#include "../RainmeterAPI/RainmeterAPI.h"

#define GetAValue(rgb)			(LOBYTE((rgb) >> 24))
#define SYSCOLOR_VERSION		((2 * 1000000) + (0 * 1000) + 0)
#define SYSCOLOR_VERSIONSTR		L"2.0.0"

namespace
{

static HMODULE g_DWMApi = nullptr;
static HMODULE g_UxTheme = nullptr;
static UINT g_Instances = 0U;

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

typedef HRESULT(WINAPI* FPDWMGETCOLORIZATIONPARAMETERS)(COLORIZATIONPARAMS* pColorParams);
static FPDWMGETCOLORIZATIONPARAMETERS c_DwmGetColorizationParameters = nullptr;

typedef struct IMMERSIVE_COLOR_PREFERENCE
{
	COLORREF color1;
	COLORREF color2;
} ImmersiveColorPreference;

typedef HRESULT(WINAPI* FPGETUSERCOLORPREFERENCE)(IMMERSIVE_COLOR_PREFERENCE* pImmersivePreference, BOOL forceReload);
static FPGETUSERCOLORPREFERENCE c_GetUserColorPreference = nullptr;

enum class ColorType : int
{
	INVALID = -1,

	// Values to use with GetSysColorBrush (WinUser.h)
	// Values documented here: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getsyscolor
	// Note: Most of these are supposedly not supported by Windows 10+, but they still work (for now)
	//       even if the OS doesn't use them much anymore
	SCROLLBAR = COLOR_SCROLLBAR,
	DESKTOP = COLOR_DESKTOP,
	ACTIVECAPTION = COLOR_ACTIVECAPTION,
	INACTIVECAPTION = COLOR_INACTIVECAPTION,
	MENU = COLOR_MENU,
	WINDOW = COLOR_WINDOW,
	WINDOWFRAME = COLOR_WINDOWFRAME,
	MENUTEXT = COLOR_MENUTEXT,
	WINDOWTEXT = COLOR_WINDOWTEXT,
	CAPTIONTEXT = COLOR_CAPTIONTEXT,
	ACTIVEBORDER = COLOR_ACTIVEBORDER,
	INACTIVEBORDER = COLOR_INACTIVEBORDER,
	APPWORKSPACE = COLOR_APPWORKSPACE,
	HIGHLIGHT = COLOR_HIGHLIGHT,
	HIGHLIGHTTEXT = COLOR_HIGHLIGHTTEXT,
	BUTTONFACE = COLOR_BTNFACE,
	BUTTONSHADOW = COLOR_BTNSHADOW,
	GRAYTEXT = COLOR_GRAYTEXT,
	BUTTONTEXT = COLOR_BTNTEXT,
	INACTIVECAPTIONTEXT = COLOR_INACTIVECAPTIONTEXT,
	BUTTONHIGHLIGHT = COLOR_BTNHIGHLIGHT,
	DDARKSHADOW = COLOR_3DDKSHADOW,
	DLIGHT = COLOR_3DLIGHT,
	INFOTEXT = COLOR_INFOTEXT,
	INFOBACKGROUND = COLOR_INFOBK,
	HOTLIGHT = COLOR_HOTLIGHT,
	GRADIENTACTIVECAPTION = COLOR_GRADIENTACTIVECAPTION,
	GRADIENTINACTIVECAPTION = COLOR_GRADIENTINACTIVECAPTION,
	MENUHIGHLIGHT = COLOR_MENUHILIGHT,
	MENUBAR = COLOR_MENUBAR,

	// Windows 7 color used for DWM glass composition
	// See: https://learn.microsoft.com/en-us/windows/win32/api/dwmapi/nf-dwmapi-dwmgetcolorizationcolor
	WIN7_AERO = 100,

	// Windows 10/11 accent color
	// Retrieved from uxtheme.dll:GetUserColorPreference
	ACCENT = 200,

	// Raw DWM values retrived from the undocumented function "DwmGetColorizationParameters"
	// Note: |WIN8_WINDOW| simulates how Windows 8/8.1 calculates its window color
	WIN8_WINDOW = 300,
	DWM_COLORIZATION_COLOR,
	DWM_AFTERGLOW_COLOR,
	DWM_COLOR_BALANCE,
	DWM_AFTERGLOW_BALANCE,
	DWM_BLUR_BALANCE,
	DWM_GLASS_REFLECTION_INTENSITY,
	DWM_OPAQUE_BLEND
};

enum class DisplayType : UINT
{
	ALL = 0U,		// Returns the entire color (can be without alpha channel depending on ColorType)
	RED,			// Returns only the Red channel of the color
	GREEN,			// Returns only the Green channel of the color
	BLUE,			// Returns only the Blue channel of the color
	ALPHA,			// Returns only the Alpha channel
	RGB				// Returns tthe entire color without the Alpha channel
};

struct Measure
{
	std::wstring color;
	bool isHex;

	ColorType colorType;
	DisplayType displayType;

	Measure() :
		color(),
		isHex(false),
		colorType(ColorType::INVALID),
		displayType(DisplayType::ALL)
	{ }
};

std::wstring ColorToString(const int color, bool hex)
{
	WCHAR buffer[5] = { 0 };

	if (hex)
	{
		_snwprintf_s(buffer, _TRUNCATE, L"%02X", color);
	}
	else
	{
		_snwprintf_s(buffer, _TRUNCATE, L"%d", color);
	}

	return buffer;
}

COLORREF ToCOLORREF(DWORD argb)
{
	// Converts 0xAARRGGBB format to 0xAABBGGRR
	return
		((argb & 0x00FF0000) >> 16) |  //______RR
		((argb & 0x0000FF00))       |  //____GG__
		((argb & 0x000000FF) << 16) |  //__BB____
		((argb & 0xFF000000));         //AA______
}

std::wstring Widen(const char* str, int strLen = -1, int cp = CP_ACP)
{
	std::wstring wideStr;

	if (str && *str)
	{
		if (strLen == -1)
		{
			strLen = (int)strlen(str);
		}

		int bufLen = MultiByteToWideChar(cp, 0, str, strLen, nullptr, 0);
		if (bufLen > 0)
		{
			wideStr.resize(bufLen);
			MultiByteToWideChar(cp, 0, str, strLen, &wideStr[0], bufLen);
		}
	}
	return wideStr;
}

void CheckVersion(void* rm)
{
	HINTERNET hRootHandle = InternetOpen(L"Rainmeter: SysColor.dll", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, 0);
	if (!hRootHandle) return;

	const WCHAR* url = L"https://brianferguson.github.io/SysColor.dll/version";
	HINTERNET hUrlDump = InternetOpenUrl(hRootHandle, url, nullptr, 0, INTERNET_FLAG_RESYNCHRONIZE, 0);
	if (hUrlDump)
	{
		DWORD dwSize = 0UL;
		char urlData[16] = { 0 };
		if (InternetReadFile(hUrlDump, (LPVOID)urlData, sizeof(urlData) - 1, &dwSize))
		{
			auto parseVersion = [](const WCHAR* str) -> int
			{
				int version = _wtoi(str) * 1000000;
				const WCHAR* pos = wcschr(str, L'.');
				if (pos)
				{
					++pos;	// Skip .
					version += _wtoi(pos) * 1000;

					pos = wcschr(pos, '.');
					if (pos)
					{
						++pos;	// Skip .
						version += _wtoi(pos);
					}
				}
				return version;
			};

			std::wstring version = Widen(urlData);

			int availableVersion = parseVersion(version.c_str());
			if (availableVersion > SYSCOLOR_VERSION)
			{
				if (rm)
				{
					RmLogF(rm, LOG_NOTICE, L"SysColor.dll: Version %s available! Installed version: %s", version.c_str(), SYSCOLOR_VERSIONSTR);
				}
			}
		}
		InternetCloseHandle(hUrlDump);
	}

	InternetCloseHandle(hRootHandle);
}

};  // namespace

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;

	if (g_Instances == 0U)
	{
		_beginthread(CheckVersion, 0, rm);

		SetDllDirectory(L"");
		SetLastError(ERROR_SUCCESS);

		g_DWMApi = LoadLibrary(L"dwmapi.dll");
		if (g_DWMApi)
		{
			c_DwmGetColorizationParameters = (FPDWMGETCOLORIZATIONPARAMETERS)GetProcAddress(g_DWMApi, (LPCSTR)127);  // Undocumented
			if (!c_DwmGetColorizationParameters)
			{
				RmLogF(rm, LOG_ERROR, L"SysColor: Cound not find \"DwmGetColorizationParameters\" (dwmapi.dll)");
			}
		}
		else
		{
			RmLogF(rm, LOG_ERROR, L"SysColor: Could not load \"dwmapi.dll\"");
		}

		g_UxTheme = LoadLibrary(L"uxtheme.dll");
		if (g_UxTheme)
		{
			c_GetUserColorPreference = (FPGETUSERCOLORPREFERENCE)GetProcAddress(g_UxTheme, "GetUserColorPreference");
			if (!c_GetUserColorPreference)
			{
				RmLogF(rm, LOG_ERROR, L"SysColor: Cound not find \"GetUserColorPreference\" (uxtheme.dll)");
			}
		}
		else
		{
			RmLogF(rm, LOG_ERROR, L"SysColor: Could not load \"uxtheme.dll\"");
		}
	}
	++g_Instances;
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;

	{
		DisplayType oldDisplayType = measure->displayType;
		measure->displayType = DisplayType::ALL;

		LPCWSTR displayType = RmReadString(rm, L"DisplayType", L"ALL");
		if (_wcsicmp(L"ALL", displayType) == 0)
		{
			measure->displayType = DisplayType::ALL;
		}
		else if (_wcsicmp(L"RED", displayType) == 0)
		{
			measure->displayType = DisplayType::RED;
		}
		else if (_wcsicmp(L"GREEN", displayType) == 0)
		{
			measure->displayType = DisplayType::GREEN;
		}
		else if (_wcsicmp(L"BLUE", displayType) == 0)
		{
			measure->displayType = DisplayType::BLUE;
		}
		else if (_wcsicmp(L"ALPHA", displayType) == 0)
		{
			measure->displayType = DisplayType::ALPHA;
		}
		else if (_wcsicmp(L"RGB", displayType) == 0)
		{
			measure->displayType = DisplayType::RGB;
		}
		else if (oldDisplayType != measure->displayType)
		{
			RmLogF(rm, LOG_ERROR, L"Unknown DisplayType: %s", displayType);
		}
	}

	{
		ColorType oldColorType = measure->colorType;
		measure->colorType = ColorType::INVALID;

		LPCWSTR colorType = RmReadString(rm, L"ColorType", L"ACCENT");
		if (_wcsicmp(L"SCROLLBAR", colorType) == 0)
		{
			measure->colorType = ColorType::SCROLLBAR;
		}
		else if (_wcsicmp(L"DESKTOP", colorType) == 0)
		{
			measure->colorType = ColorType::DESKTOP;
		}
		else if (_wcsicmp(L"ACTIVECAPTION", colorType) == 0)
		{
			measure->colorType = ColorType::ACTIVECAPTION;
		}
		else if (_wcsicmp(L"INACTIVECAPTION", colorType) == 0)
		{
			measure->colorType = ColorType::INACTIVECAPTION;
		}
		else if (_wcsicmp(L"MENU", colorType) == 0)
		{
			measure->colorType = ColorType::MENU;
		}
		else if (_wcsicmp(L"WINDOW", colorType) == 0)
		{
			measure->colorType = ColorType::WINDOW;
		}
		else if (_wcsicmp(L"WINDOWFRAME", colorType) == 0)
		{
			measure->colorType = ColorType::WINDOWFRAME;
		}
		else if (_wcsicmp(L"MENUTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::MENUTEXT;
		}
		else if (_wcsicmp(L"WINDOWTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::WINDOWTEXT;
		}
		else if (_wcsicmp(L"CAPTIONTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::CAPTIONTEXT;
		}
		else if (_wcsicmp(L"ACTIVEBORDER", colorType) == 0)
		{
			measure->colorType = ColorType::ACTIVEBORDER;
		}
		else if (_wcsicmp(L"INACTIVEBORDER", colorType) == 0)
		{
			measure->colorType = ColorType::INACTIVEBORDER;
		}
		else if (_wcsicmp(L"APPWORKSPACE", colorType) == 0)
		{
			measure->colorType = ColorType::APPWORKSPACE;
		}
		else if (_wcsicmp(L"HIGHLIGHT", colorType) == 0)
		{
			measure->colorType = ColorType::HIGHLIGHT;
		}
		else if (_wcsicmp(L"HIGHLIGHTTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::HIGHLIGHTTEXT;
		}
		else if (_wcsicmp(L"BUTTONFACE", colorType) == 0)
		{
			measure->colorType = ColorType::BUTTONFACE;
		}
		else if (_wcsicmp(L"BUTTONSHADOW", colorType) == 0)
		{
			measure->colorType = ColorType::BUTTONSHADOW;
		}
		else if (_wcsicmp(L"GRAYTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::GRAYTEXT;
		}
		else if (_wcsicmp(L"BUTTONTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::BUTTONTEXT;
		}
		else if (_wcsicmp(L"INACTIVECAPTIONTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::INACTIVECAPTIONTEXT;
		}
		else if (_wcsicmp(L"BUTTONHIGHLIGHT", colorType) == 0)
		{
			measure->colorType = ColorType::BUTTONHIGHLIGHT;
		}
		else if (_wcsicmp(L"3DDARKSHADOW", colorType) == 0)
		{
			measure->colorType = ColorType::DDARKSHADOW;
		}
		else if (_wcsicmp(L"3DLIGHT", colorType) == 0)
		{
			measure->colorType = ColorType::DLIGHT;
		}
		else if (_wcsicmp(L"TOOLTIPTEXT", colorType) == 0)
		{
			measure->colorType = ColorType::INFOTEXT;
		}
		else if (_wcsicmp(L"TOOLTIPBACKGROUND", colorType) == 0)
		{
			measure->colorType = ColorType::INFOBACKGROUND;
		}
		else if (_wcsicmp(L"HYPERLINK", colorType) == 0)
		{
			measure->colorType = ColorType::HOTLIGHT;
		}
		else if (_wcsicmp(L"ACTIVECAPTIONGRADIENT", colorType) == 0)
		{
			measure->colorType = ColorType::GRADIENTACTIVECAPTION;
		}
		else if (_wcsicmp(L"INACTIVECAPTIONGRADIENT", colorType) == 0)
		{
			measure->colorType = ColorType::GRADIENTINACTIVECAPTION;
		}
		else if (_wcsicmp(L"MENUHIGHLIGHT", colorType) == 0)
		{
			measure->colorType = ColorType::MENUHIGHLIGHT;
		}
		else if (_wcsicmp(L"MENUBAR", colorType) == 0)
		{
			measure->colorType = ColorType::MENUBAR;
		}

		// Windows 7
		else if (_wcsicmp(L"AERO", colorType) == 0)
		{
			measure->colorType = ColorType::WIN7_AERO;
		}

		// Windows 10/11
		else if (_wcsicmp(L"ACCENT", colorType) == 0)
		{
			if (c_GetUserColorPreference && IsWindows10OrGreater())
			{
				measure->colorType = ColorType::ACCENT;
			}
			else
			{
				measure->colorType = ColorType::WIN7_AERO;
				RmLog(rm, LOG_WARNING, L"SysColor: \"ColorType=Accent\" not available");
			}
		}

		// Raw DWM values retrived from the undocumented function "DwmGetColorizationParameters"
		else if (_wcsicmp(L"WIN8", colorType) == 0)
		{
			measure->colorType = ColorType::WIN8_WINDOW;
		}
		else if (_wcsicmp(L"DWM_COLOR", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_COLORIZATION_COLOR;
		}
		else if (_wcsicmp(L"DWM_AFTERGLOW_COLOR", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_AFTERGLOW_COLOR;
		}
		else if (_wcsicmp(L"DWM_COLOR_BALANCE", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_COLOR_BALANCE;
		}
		else if (_wcsicmp(L"DWM_AFTERGLOW_BALANCE", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_AFTERGLOW_BALANCE;
		}
		else if (_wcsicmp(L"DWM_BLUR_BALANCE", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_BLUR_BALANCE;
		}
		else if (_wcsicmp(L"DWM_GLASS_REFLECTION_INTENSITY", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_GLASS_REFLECTION_INTENSITY;
		}
		else if (_wcsicmp(L"DWM_OPAQUE_BLEND", colorType) == 0)
		{
			measure->colorType = ColorType::DWM_OPAQUE_BLEND;
		}
		else if (oldColorType != measure->colorType)
		{
			RmLogF(rm, LOG_ERROR, L"Unknown ColorType: %s", colorType);
		}
	}

	measure->isHex = 0 != RmReadInt(rm, L"Hex", 0);
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;

	if (measure->colorType == ColorType::INVALID)
	{
		measure->color.clear();
		return -1.0;
	}

	int r = 0, g = 0, b = 0, a = 0;

	if (measure->colorType == ColorType::WIN7_AERO)
	{
		DWORD color = 0UL;
		BOOL opaque = FALSE;

		// Color stored in 0xAARRGGBB format
		HRESULT hr = DwmGetColorizationColor(&color, &opaque);
		if (SUCCEEDED(hr))
		{
			DWORD abgr = ToCOLORREF(color);
			r = GetRValue(abgr);
			g = GetGValue(abgr);
			b = GetBValue(abgr);
			a = GetAValue(abgr);
		}
		else
		{
			measure->color.clear();
			return -1.0;
		}
	}

	// Windows 10/11
	else if (measure->colorType == ColorType::ACCENT)
	{
		if (!g_UxTheme)
		{
			measure->color.clear();
			return -1.0;
		}

		IMMERSIVE_COLOR_PREFERENCE immersiveColorPreference = { 0 };
		HRESULT hr = c_GetUserColorPreference(&immersiveColorPreference, FALSE);
		if (FAILED(hr))
		{
			measure->color.clear();
			return -1.0;
		}

		r = GetRValue(immersiveColorPreference.color2);
		g = GetGValue(immersiveColorPreference.color2);
		b = GetBValue(immersiveColorPreference.color2);
		a = GetAValue(immersiveColorPreference.color2);
	}

	// Raw DWM values (and WIN8_WINDOW)
	else if (measure->colorType >= ColorType::WIN8_WINDOW)
	{
		if (!g_DWMApi)  // Just in case
		{
			measure->color.clear();
			return -1.0;
		}

		BOOL isEnabled = FALSE;
		HRESULT hr = DwmIsCompositionEnabled(&isEnabled);
		if (FAILED(hr))
		{
			measure->color.clear();
			return -1.0;
		}

		DWMColorizationParameters params;
		hr = c_DwmGetColorizationParameters(&params);
		if (FAILED(hr))
		{
			measure->color.clear();
			return -1.0;
		}

		// COLORREF is stored in 0xAABBGGRR format, but the color is stored in 0xAARRGGBB format.
		DWORD color = ToCOLORREF((measure->colorType == ColorType::DWM_AFTERGLOW_COLOR) ?
			params.colorizationAfterglow : params.colorizationColor);
		r = GetRValue(color);
		g = GetGValue(color);
		b = GetBValue(color);
		a = GetAValue(color);

		switch (measure->colorType)
		{
		case ColorType::WIN8_WINDOW:
			{
				double bal = 100.0 - params.colorizationColorBalance;

				r = min((int)round(r + (217 - r) * bal / 100.0), 255);
				g = min((int)round(g + (217 - g) * bal / 100.0), 255);
				b = min((int)round(b + (217 - b) * bal / 100.0), 255);
			}
			break;

		case ColorType::DWM_COLORIZATION_COLOR:
		case ColorType::DWM_AFTERGLOW_COLOR:
			// Values already calculated
			break;

		case ColorType::DWM_COLOR_BALANCE:
			measure->color = std::to_wstring(params.colorizationColorBalance);
			return 1.0;

		case ColorType::DWM_AFTERGLOW_BALANCE:
			measure->color = std::to_wstring(params.colorizationAfterglowBalance);
			return 1.0;

		case ColorType::DWM_BLUR_BALANCE:
			measure->color = std::to_wstring(params.colorizationBlurBalance);
			return 1.0;

		case ColorType::DWM_GLASS_REFLECTION_INTENSITY:
			measure->color = std::to_wstring(params.colorizationGlassReflectionIntensity);
			return 1.0;

		case ColorType::DWM_OPAQUE_BLEND:
			measure->color = std::to_wstring(params.colorizationOpaqueBlend);
			return 1.0;
		}
	}

	// GetSysColorBrush
	else
	{
		HBRUSH hBrush = GetSysColorBrush((int)measure->colorType);
		LOGBRUSH lb = { 0 };
		if (GetObject(hBrush, sizeof(LOGBRUSH), (LPSTR)&lb) <= 0 || !hBrush)
		{
			if (hBrush) DeleteObject(hBrush);

			measure->color.clear();
			return -1.0;
		}

		r = GetRValue(lb.lbColor);
		g = GetGValue(lb.lbColor);
		b = GetBValue(lb.lbColor);
		a = 0;

		DeleteObject(hBrush);
	}

	bool hex = measure->isHex;
	switch (measure->displayType)
	{
	case DisplayType::RED:
		measure->color = ColorToString(r, hex);
		break;

	case DisplayType::GREEN:
		measure->color = ColorToString(g, hex);
		break;

	case DisplayType::BLUE:
		measure->color = ColorToString(b, hex);
		break;

	case DisplayType::ALPHA:
		if (a)
		{
			measure->color = ColorToString(a, hex);
			break;
		}
		measure->color.clear();
		return -1.0;

	case DisplayType::RGB:
		measure->color = ColorToString(r, hex);  // Red
		if (!hex) measure->color += L",";

		measure->color += ColorToString(g, hex);  // Green
		if (!hex) measure->color += L",";

		measure->color += ColorToString(b, hex);  // Blue
		break;

	case DisplayType::ALL:
		measure->color = ColorToString(r, hex);  // Red
		if (!hex) measure->color += L",";

		measure->color += ColorToString(g, hex);  // Green
		if (!hex) measure->color += L",";

		measure->color += ColorToString(b, hex);  // Blue

		if (a > 0)
		{
			if (!hex) measure->color += L",";
			measure->color += ColorToString(a, hex);  // Alpha
		}
		break;
	}

	return measure->color.empty() ? -1.0 : 1.0;
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
	Measure* measure = (Measure*)data;
	return measure->color.empty() ? L"" : measure->color.c_str();
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;

	delete measure;
	measure = nullptr;

	--g_Instances;

	if (g_Instances == 0U)
	{
		if (g_DWMApi)
		{
			FreeLibrary(g_DWMApi);
			g_DWMApi = nullptr;
		}
		c_DwmGetColorizationParameters = nullptr;

		if (g_UxTheme)
		{
			FreeLibrary(g_UxTheme);
			g_UxTheme = nullptr;
		}
		c_GetUserColorPreference = nullptr;
	}
}
