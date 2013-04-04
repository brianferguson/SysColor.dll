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

#include "PluginSysColor.h"

HMODULE hLib;
static std::vector<Measure*> g_Measures;

std::wstring ColorToString(const int color, bool hex);

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;

	g_Measures.push_back(measure);

	OSVERSIONINFOEX os = {sizeof(OSVERSIONINFOEX)};
	if (GetVersionEx((OSVERSIONINFO*)&os))
	{
		measure->isXP = (os.dwMajorVersion < 6) ? true : false;
	}

	if (!measure->isXP)
	{
		SetDllDirectory(L"");
		SetLastError(ERROR_SUCCESS);

		hLib = LoadLibrary(L"dwmapi.dll");
		if (hLib)
		{
			c_DwmGetColorizationColor = (FPDWMGETCOLORIZATIONCOLOR)GetProcAddress(hLib, "DwmGetColorizationColor");
		}
	}
}

PLUGIN_EXPORT void Reload(void* data, void* rm, double* maxValue)
{
	Measure* measure = (Measure*)data;

	LPCWSTR dType = RmReadString(rm, L"DisplayType", L"ALL");
	if (_wcsicmp(L"ALL", dType) == 0)
	{
		measure->displayType = DISPLAY_ALL;
	}
	else if (_wcsicmp(L"RED", dType) == 0)
	{
		measure->displayType = DISPLAY_RED;
	}
	else if (_wcsicmp(L"GREEN", dType) == 0)
	{
		measure->displayType = DISPLAY_GREEN;
	}
	else if (_wcsicmp(L"BLUE", dType) == 0)
	{
		measure->displayType = DISPLAY_BLUE;
	}
	else if (_wcsicmp(L"ALPHA", dType) == 0)
	{
		measure->displayType = DISPLAY_ALPHA;
	}
	else if (_wcsicmp(L"RGB", dType) == 0)
	{
		measure->displayType = DISPLAY_RGB;
	}
	else
	{
		RmLog(LOG_ERROR, L"SysColor.dll: Unknown display type");
	}

	LPCWSTR cType = RmReadString(rm, L"ColorType", L"DESKTOP");
	if (_wcsicmp(L"SCROLLBAR", cType) == 0)
	{
		measure->colorType = SYSCOLOR_SCROLLBAR;
	}
	else if (_wcsicmp(L"DESKTOP", cType) == 0)
	{
		measure->colorType = SYSCOLOR_DESKTOP;
	}
	else if (_wcsicmp(L"ACTIVECAPTION", cType) == 0)
	{
		measure->colorType = SYSCOLOR_ACTIVECAPTION;
	}
	else if (_wcsicmp(L"INACTIVECAPTION", cType) == 0)
	{
		measure->colorType = SYSCOLOR_INACTIVECAPTION;
	}
	else if (_wcsicmp(L"MENU", cType) == 0)
	{
		measure->colorType = SYSCOLOR_MENU;
	}
	else if (_wcsicmp(L"WINDOW", cType) == 0)
	{
		measure->colorType = SYSCOLOR_WINDOW;
	}
	else if (_wcsicmp(L"WINDOWFRAME", cType) == 0)
	{
		measure->colorType = SYSCOLOR_WINDOWFRAME;
	}
	else if (_wcsicmp(L"MENUTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_MENUTEXT;
	}
	else if (_wcsicmp(L"WINDOWTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_WINDOWTEXT;
	}
	else if (_wcsicmp(L"CAPTIONTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_CAPTIONTEXT;
	}
	else if (_wcsicmp(L"ACTIVEBORDER", cType) == 0)
	{
		measure->colorType = SYSCOLOR_ACTIVEBORDER;
	}
	else if (_wcsicmp(L"INACTIVEBORDER", cType) == 0)
	{
		measure->colorType = SYSCOLOR_INACTIVEBORDER;
	}
	else if (_wcsicmp(L"APPWORKSPACE", cType) == 0)
	{
		measure->colorType = SYSCOLOR_APPWORKSPACE;
	}
	else if (_wcsicmp(L"HIGHLIGHT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_HIGHLIGHT;
	}
	else if (_wcsicmp(L"HIGHLIGHTTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_HIGHLIGHTTEXT;
	}
	else if (_wcsicmp(L"BUTTONFACE", cType) == 0)
	{
		measure->colorType = SYSCOLOR_BUTTONFACE;
	}
	else if (_wcsicmp(L"BUTTONSHADOW", cType) == 0)
	{
		measure->colorType = SYSCOLOR_BUTTONSHADOW;
	}
	else if (_wcsicmp(L"GRAYTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_GRAYTEXT;
	}
	else if (_wcsicmp(L"BUTTONTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_BUTTONTEXT;
	}
	else if (_wcsicmp(L"INACTIVECAPTIONTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_INACTIVECAPTIONTEXT;
	}
	else if (_wcsicmp(L"BUTTONHIGHLIGHT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_BUTTONHIGHLIGHT;
	}
	else if (_wcsicmp(L"3DDARKSHADOW", cType) == 0)
	{
		measure->colorType = SYSCOLOR_3DDARKSHADOW;
	}
	else if (_wcsicmp(L"3DLIGHT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_3DLIGHT;
	}
	else if (_wcsicmp(L"TOOLTIPTEXT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_INFOTEXT;
	}
	else if (_wcsicmp(L"TOOLTIPBACKGROUND", cType) == 0)
	{
		measure->colorType = SYSCOLOR_INFOBACKGROUND;
	}
	else if (_wcsicmp(L"AERO", cType) == 0)
	{
		if (!measure->isXP)
		{
			measure->colorType = SYSCOLOR_GLASS;
		}
		else
		{
			RmLog(LOG_ERROR, L"SysColor.dll: Windows XP does not support ColorType=Aero");
		}
	}
	else if (_wcsicmp(L"HYPERLINK", cType) == 0)
	{
		measure->colorType = SYSCOLOR_HOTLIGHT;
	}
	else if (_wcsicmp(L"ACTIVECAPTIONGRADIENT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_ACTIVECAPTIONGRADIENT;
	}
	else if (_wcsicmp(L"INACTIVECAPTIONGRADIENT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_INACTIVECAPTIONGRADIENT;
	}
	else if (_wcsicmp(L"MENUHIGHLIGHT", cType) == 0)
	{
		measure->colorType = SYSCOLOR_MENUHIGHLIGHT;
	}
	else if (_wcsicmp(L"MENUBAR", cType) == 0)
	{
		measure->colorType = SYSCOLOR_MENUBAR;
	}
	else
	{
		RmLog(LOG_ERROR, L"SysColor.dll: Unknown color type");
	}

	measure->isHex = 0!=RmReadInt(rm, L"Hex", 0);
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;
	int r = 0, g = 0, b = 0, a = 0;

	if (measure->colorType == SYSCOLOR_GLASS)
	{
		if (measure->isXP)
		{
			measure->color.clear();
			return -1.0;
		}

		DWORD color = 0;
		BOOL opaque = FALSE;

		HRESULT hr = c_DwmGetColorizationColor(&color, &opaque);
		if (SUCCEEDED(hr))
		{
			b = (color & 255);
			g = ((color >> 8) & 255);
			r = ((color >> 16) & 255);
			a = ((color >> 24) & 255);
		}
		else
		{
			measure->color.clear();
			return -1.0;
		}
	}
	else
	{
		HBRUSH hBrush = GetSysColorBrush(measure->colorType);
		LOGBRUSH lb;

		if (GetObject(hBrush, sizeof(LOGBRUSH), (LPSTR)&lb))
		{
			r = GetRValue(lb.lbColor);
			g = GetGValue(lb.lbColor);
			b = GetBValue(lb.lbColor);
			a = NULL;
		}
		else
		{
			measure->color.clear();
			return -1.0;
		}

		DeleteObject(hBrush);
	}

	bool hex = measure->isHex;

	switch (measure->displayType)
	{
	case DISPLAY_RED:
		measure->color = ColorToString(r, hex);
		break;

	case DISPLAY_GREEN:
		measure->color = ColorToString(g, hex);
		break;

	case DISPLAY_BLUE:
		measure->color = ColorToString(b, hex);
		break;

	case DISPLAY_ALPHA:
		{
			if (a)
			{
				measure->color = ColorToString(a, hex);
			}
			else
			{
				measure->color.clear();
				return -1.0;
			}
		}		
		break;

	case DISPLAY_RGB:
		{
			// Red
			measure->color = ColorToString(r, hex);
			if (!hex) measure->color += L",";

			// Green
			measure->color += ColorToString(g, hex);
			if (!hex) measure->color += L",";

			// Blue
			measure->color += ColorToString(b, hex);
		}
		break;

	case DISPLAY_ALL:
	default:
		{
			// Red
			measure->color = ColorToString(r, hex);
			if (!hex) measure->color += L",";

			// Green
			measure->color += ColorToString(g, hex);
			if (!hex) measure->color += L",";

			// Blue
			measure->color += ColorToString(b, hex);

			// Alpha
			if (a)
			{
				if (!hex) measure->color += L",";
				measure->color += ColorToString(a, hex);
			}
		}
		break;
	}

	if (!measure->color.empty())
	{
		return 1.0;
	}

	return -1.0;
}

PLUGIN_EXPORT LPCWSTR GetString(void* data)
{
	Measure* measure = (Measure*)data;

	if (!measure->color.empty())
	{
		return measure->color.c_str();
	}

	return L"";
}

PLUGIN_EXPORT void Finalize(void* data)
{
	Measure* measure = (Measure*)data;
	delete measure;

	auto iter = std::find(g_Measures.begin(), g_Measures.end(), measure);
	g_Measures.erase(iter);

	if (g_Measures.empty())
	{
		FreeLibrary(hLib);
		hLib = nullptr;
		c_DwmGetColorizationColor = nullptr;
	}
}

std::wstring ColorToString(const int color, bool hex)
{
	WCHAR buffer[255];

	if (hex)
	{
		swprintf_s(buffer, L"%02X", color);
	}
	else
	{
		swprintf_s(buffer, L"%i", color);
	}
	
	return buffer;
}