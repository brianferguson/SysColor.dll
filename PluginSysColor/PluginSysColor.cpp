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

#include "PluginSysColor.h"

HMODULE hLib;
static std::vector<Measure*> g_Measures;

inline std::wstring ColorToString(const int color, bool hex);

PLUGIN_EXPORT void Initialize(void** data, void* rm)
{
	Measure* measure = new Measure;
	*data = measure;

	measure->rm = rm;
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
			c_DwmGetColorizationParameters = (FPDWMGETCOLORIZATIONPARAMETERS)GetProcAddress(hLib, (LPCSTR)127);	// Undocumented!
			c_DwmIsCompositionEnabled = (FPDWMISCOMPOSITIONENABLED)GetProcAddress(hLib, "DwmIsCompositionEnabled");
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
		measure->displayType = ALL;
	}
	else if (_wcsicmp(L"RED", dType) == 0)
	{
		measure->displayType = RED;
	}
	else if (_wcsicmp(L"GREEN", dType) == 0)
	{
		measure->displayType = GREEN;
	}
	else if (_wcsicmp(L"BLUE", dType) == 0)
	{
		measure->displayType = BLUE;
	}
	else if (_wcsicmp(L"ALPHA", dType) == 0)
	{
		measure->displayType = ALPHA;
	}
	else if (_wcsicmp(L"RGB", dType) == 0)
	{
		measure->displayType = RGB;
	}
	else
	{
		RmLogF(rm, LOG_ERROR, L"Unknown DisplayType: %s", dType);
	}

	bool isColorSupported = true;
	LPCWSTR cType = RmReadString(rm, L"ColorType", L"DESKTOP");
	if (_wcsicmp(L"SCROLLBAR", cType) == 0)
	{
		measure->colorType = SCROLLBAR;
	}
	else if (_wcsicmp(L"DESKTOP", cType) == 0)
	{
		measure->colorType = DESKTOP;
	}
	else if (_wcsicmp(L"ACTIVECAPTION", cType) == 0)
	{
		measure->colorType = ACTIVECAPTION;
	}
	else if (_wcsicmp(L"INACTIVECAPTION", cType) == 0)
	{
		measure->colorType = INACTIVECAPTION;
	}
	else if (_wcsicmp(L"MENU", cType) == 0)
	{
		measure->colorType = MENU;
	}
	else if (_wcsicmp(L"WINDOW", cType) == 0)
	{
		measure->colorType = WINDOW;
	}
	else if (_wcsicmp(L"WINDOWFRAME", cType) == 0)
	{
		measure->colorType = WINDOWFRAME;
	}
	else if (_wcsicmp(L"MENUTEXT", cType) == 0)
	{
		measure->colorType = MENUTEXT;
	}
	else if (_wcsicmp(L"WINDOWTEXT", cType) == 0)
	{
		measure->colorType = WINDOWTEXT;
	}
	else if (_wcsicmp(L"CAPTIONTEXT", cType) == 0)
	{
		measure->colorType = CAPTIONTEXT;
	}
	else if (_wcsicmp(L"ACTIVEBORDER", cType) == 0)
	{
		measure->colorType = ACTIVEBORDER;
	}
	else if (_wcsicmp(L"INACTIVEBORDER", cType) == 0)
	{
		measure->colorType = INACTIVEBORDER;
	}
	else if (_wcsicmp(L"APPWORKSPACE", cType) == 0)
	{
		measure->colorType = APPWORKSPACE;
	}
	else if (_wcsicmp(L"HIGHLIGHT", cType) == 0)
	{
		measure->colorType = HIGHLIGHT;
	}
	else if (_wcsicmp(L"HIGHLIGHTTEXT", cType) == 0)
	{
		measure->colorType = HIGHLIGHTTEXT;
	}
	else if (_wcsicmp(L"BUTTONFACE", cType) == 0)
	{
		measure->colorType = BUTTONFACE;
	}
	else if (_wcsicmp(L"BUTTONSHADOW", cType) == 0)
	{
		measure->colorType = BUTTONSHADOW;
	}
	else if (_wcsicmp(L"GRAYTEXT", cType) == 0)
	{
		measure->colorType = GRAYTEXT;
	}
	else if (_wcsicmp(L"BUTTONTEXT", cType) == 0)
	{
		measure->colorType = BUTTONTEXT;
	}
	else if (_wcsicmp(L"INACTIVECAPTIONTEXT", cType) == 0)
	{
		measure->colorType = INACTIVECAPTIONTEXT;
	}
	else if (_wcsicmp(L"BUTTONHIGHLIGHT", cType) == 0)
	{
		measure->colorType = BUTTONHIGHLIGHT;
	}
	else if (_wcsicmp(L"3DDARKSHADOW", cType) == 0)
	{
		measure->colorType = DDARKSHADOW;
	}
	else if (_wcsicmp(L"3DLIGHT", cType) == 0)
	{
		measure->colorType = DLIGHT;
	}
	else if (_wcsicmp(L"TOOLTIPTEXT", cType) == 0)
	{
		measure->colorType = INFOTEXT;
	}
	else if (_wcsicmp(L"TOOLTIPBACKGROUND", cType) == 0)
	{
		measure->colorType = INFOBACKGROUND;
	}
	else if (_wcsicmp(L"HYPERLINK", cType) == 0)
	{
		measure->colorType = HOTLIGHT;
	}
	else if (_wcsicmp(L"ACTIVECAPTIONGRADIENT", cType) == 0)
	{
		measure->colorType = ACTIVECAPTIONGRADIENT;
	}
	else if (_wcsicmp(L"INACTIVECAPTIONGRADIENT", cType) == 0)
	{
		measure->colorType = INACTIVECAPTIONGRADIENT;
	}
	else if (_wcsicmp(L"MENUHIGHLIGHT", cType) == 0)
	{
		measure->colorType = MENUHIGHLIGHT;
	}
	else if (_wcsicmp(L"MENUBAR", cType) == 0)
	{
		measure->colorType = MENUBAR;
	}
	else if (_wcsicmp(L"AERO", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = WIN7_AERO;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"WIN8", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = WIN8_WINDOW;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_COLOR", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_COLORIZATION_COLOR;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_AFTERGLOW_COLOR", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_AFTERGLOW_COLOR;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_COLOR_BALANCE", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_COLOR_BALANCE;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_AFTERGLOW_BALANCE", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_AFTERGLOW_BALANCE;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_BLUR_BALANCE", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_BLUR_BALANCE;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_GLASS_REFLECTION_INTENSITY", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_GLASS_REFLECTION_INTENSITY;
		else isColorSupported = false;
	}
	else if (_wcsicmp(L"DWM_OPAQUE_BLEND", cType) == 0)
	{
		if (!measure->isXP) measure->colorType = DWM_OPAQUE_BLEND;
		else isColorSupported = false;
	}
	else
	{
		measure->colorType = INVALID;
		RmLogF(rm, LOG_ERROR, L"Unknown ColorType: %s", cType);
	}

	if (!isColorSupported)
	{
		measure->colorType = INVALID;
		RmLogF(rm, LOG_ERROR, L"\"ColorType=%s\" is not supported by Windows XP.", cType);
	}

	measure->isHex = 0!=RmReadInt(rm, L"Hex", 0);
}

PLUGIN_EXPORT double Update(void* data)
{
	Measure* measure = (Measure*)data;

	if (measure->colorType == INVALID)
	{
		measure->color.clear();
		return -1.0;
	}

	int r = 0, g = 0, b = 0, a = 0;

	if (measure->colorType == WIN7_AERO)
	{
		DWORD color = 0;
		BOOL opaque = FALSE;

		// Color stored in 0xAARRGGBB format
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
	else if (measure->colorType >= WIN8_WINDOW)
	{
		BOOL isEnabled;
		HRESULT hr = c_DwmIsCompositionEnabled(&isEnabled);
		if (SUCCEEDED(hr))
		{
			DWMColorizationParameters params;
			hr = c_DwmGetColorizationParameters(&params);
			if (SUCCEEDED(hr))
			{
				// COLORREF is stored in 0xAABBGGRR format, but the color is stored in 0xAARRGGBB format.
				DWORD color = (measure->colorType == DWM_AFTERGLOW_COLOR) ? params.colorizationAfterglow : params.colorizationColor;
				b = (color & 255);
				g = ((color >> 8) & 255);
				r = ((color >> 16) & 255);
				a = ((color >> 24) & 255);

				switch (measure->colorType)
				{
				case WIN8_WINDOW:
				{
					double bal = 100.0 - params.colorizationColorBalance;

					r = min((int)round(r + (217 - r) * bal / 100.0), 255);
					g = min((int)round(g + (217 - g) * bal / 100.0), 255);
					b = min((int)round(b + (217 - b) * bal / 100.0), 255);
				}
					break;

				case DWM_COLORIZATION_COLOR:
				case DWM_AFTERGLOW_COLOR:
					// Values already calculated
					break;

				case DWM_COLOR_BALANCE:
					measure->color = std::to_wstring(params.colorizationColorBalance);
					return 1.0;

				case DWM_AFTERGLOW_BALANCE:
					measure->color = std::to_wstring(params.colorizationAfterglowBalance);
					return 1.0;

				case DWM_BLUR_BALANCE:
					measure->color = std::to_wstring(params.colorizationBlurBalance);
					return 1.0;

				case DWM_GLASS_REFLECTION_INTENSITY:
					measure->color = std::to_wstring(params.colorizationGlassReflectionIntensity);
					return 1.0;

				case DWM_OPAQUE_BLEND:
					measure->color = std::to_wstring(params.colorizationOpaqueBlend);
					return 1.0;
				}
			}
		}

		if (FAILED(hr))
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
	case RED:
		measure->color = ColorToString(r, hex);
		break;

	case GREEN:
		measure->color = ColorToString(g, hex);
		break;

	case BLUE:
		measure->color = ColorToString(b, hex);
		break;

	case ALPHA:
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

	case RGB:
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

	case ALL:
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
		c_DwmGetColorizationParameters = nullptr;
		c_DwmIsCompositionEnabled = nullptr;
		c_DwmGetColorizationColor = nullptr;
	}
}

std::wstring ColorToString(const int color, bool hex)
{
	WCHAR buffer[5];

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
