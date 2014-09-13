SysColor.dll
=============

SysColor.dll is a plugin for [Rainmeter](http://www.rainmeter.net) that will retrieve system colors.


Contents
-

* [Features](#features)
* [Options](#options)
* [Changes](#changes)
* [Download](#download)
* [Build Instructions](#build-instructions)
* [Examples](#examples)


Features
-
Here are some of the features of the SysColor plugin:

* Retrieval of Windows 8/8.1 window color!
* Retrieval of the current Windows Aero color (including alpha transparency) for Windows Vista and above.
* Over 30 different colors available (eg. `Background`, `Highlight`, `Menu`).
* Different display modes.
* Output in hex or decimal form.
* A numeric return of "1" means the color was retrieved. A numeric value of "-1" means the color was *not* retrieved. The numeric value can be retrieved through [section variables](http://docs.rainmeter.net/manual-beta/variables/section-variables) (eg. [MeasureName:]).

#####Note:
For non-Windows XP users using an Aero theme, the Desktop Window Manager might choose which color get returned for some options.


Options
-
* **Hex** - When set to "1", the output color is in hexidecimal form. When set to "0", the output color is in decimal form separated by commas. `Hex=0` is default.
* **DisplayType** - Type of output. `DisplayType=All` is default. Options include:
  * **Red** - Output only the red value.
  * **Green** - Output only the green value.
  * **Blue** - Output only the blue value.
  * **Alpha** - Output only the alpha value (for `ColorType=Aero` only).
  * **RGB** - Output only the red, green and blue values (No alpha is output).
  * **ALL** - Output only the all values.
* **ColorType** - Type of color to retrieve. `ColorType=Desktop` is default. Options include:
  * **WIN8** - Current window color for Windows 8 and 8.1 (I recommend using `DisplayType=RGB` for this option). This option is not available for Windows XP users.
  * **Aero** - Current color of Aero theme (including alpha transparency). This option is not available for Windows XP users.
  * **Desktop** - Current color of the desktop background (when a solid color has been chosen for the background).
  * **ActiveCaption** - Left side color in the color gradient of an active window's title bar.
  * **ActiveCaptionGradient** - Right side color in the color gradient of an active window's title bar.
  * **InActiveCaption** - Left side color in the color gradient of an inactive window's title bar.
  * **InActiveCaptionGradient** - Right side color in the color gradient of an inactive window's title bar.
  * **CaptionText** - Color of text in the caption of a window (eg. title bar).
  * **InActiveCaptionText** - Color of text in an inactive caption.
  * **ActiveBorder** - Active window border color.
  * **InActiveBorder** - Inactive window border color.
  * **Highlight** - Color of selected items in a control.
  * **HightlightText** - Color of selected text in a control.
  * **Menu** - Menu background color.
  * **MenuText** - Color of text in a menu.
  * **MenuBar** - Background color for a menu bar when the menu appears as a flat menu.
  * **MenuHighlight** - Color used to highlight menu items when the menu appears as a flat menu.
  * **Window** - Window background color.
  * **WindowFrame** - Window frame color.
  * **WindowText** - Color of text in a window.
  * **ButtonFace** - Face color of a button and other 3D display elements (eg. dialog box backgrounds).
  * **ButtonText** - Color of text on a button.
  * **ButtonHighlight** - Highlight color of 3D display elements.
  * **ButtonShadow** - Shadow color of 3D display elements (facing away from the light source).
  * **3DDarkShadow** - Dark shadow for 3D display elements.
  * **3DLight** - Light color for 3D display elements (facing toward the light source).
  * **GrayText** - Color of text that is disabled (might be 0 on some displays).
  * **ToolTipBackground** - Background color of tooltip controls.
  * **ToolTipText** - Color of text for tooltip controls.
  * **AppWorkspace** - Background color of certain multiple document interface (MDI) applications.
  * **Scrollbar** - Scrollbar gray area.
  * **Hyperlink** - Color of hyperlink or hot-tracked items.
* ColorType special "raw" [DWM](http://en.wikipedia.org/wiki/Desktop_Window_Manager) values for colorization. These options are not available for Windows XP. Options include:
  * **DWM_COLOR** - Raw color DWM uses for colorization.
  * **DWM_AFTERGLOW** - Raw color DWM uses for colorization. This is usually the same value as `DWM_COLOR`.
  * **DWM_COLOR_BALANCE** - Represents the color intensity. This can adjusted this under the "Personalization\Window Color and Appearance" control panel item.
  * **DWM_COLOR_AFTERGLOW_BALANCE** - DWM calculated afterglow balance.
  * **DWM_COLOR_BLUR_BALANCE** - DWM calculated blur balance. Usually returns `1` on Windows 8/8.1 systems.
  * **DWM_GLASS_REFLECTION_INTENSITY** - DWM calculated glass reflection intensity. Usually returns `1` on Windows 8/8.1 systems.
  * **DWM_OPAQUE_BLEND** - Returns `0` if transparency is enabled on Windows Vista/7. Returns `1` if transparency is not enabled or if on Windows8/8.1 systems.
  
#####Note:
For non-Windows XP users using an Aero theme, the Desktop Window Manager might choose which color get returned for some of the above options.

Changes
-
Here is a list of the major changes to the plugin.

#####Version:
* **1.1.0.13** - Added new types. Updated to latest SDK. Updated to use VS2013.
* **1.0.2** - Fixed crash when using `ColorType=Aero` across multiple skins.
* **1.0.1** - Fixed Windows XP support.
* **1.0.0** - Initial Version.

Download
-
####To download the current source code:

* Using git: `git clone git@github.com:brianferguson/SysColor.dll.git`
* Download as a [.zip](https://github.com/brianferguson/SysColor.dll/zipball/master)

####To download current plugin (.dll):

* [32-bit version](https://github.com/brianferguson/SysColor.dll/blob/master/PluginSysColor/x32/Release/Clipboard.dll?raw=true)
* [64-bit version](https://github.com/brianferguson/SysColor.dll/blob/master/PluginSysColor/x64/Release/Clipboard.dll?raw=true)


Build Instructions
-
This plugin can be built using any version of Visual Studio 2013. If you don't already have VS2013, you can download the free "Visual Studio Express 2013 for Windows Desktop" version [here](http://www.visualstudio.com/downloads/download-visual-studio-vs).

After Visual Studio has been installed and updated, open `PluginSysColor.sln` at the root of the repository to build.


Examples
-
####Example 1:
This example will get current Windows 8/8.1 window color.

```ini
[mWindowColor]
Measure=Plugin
Plugin=SysColor
ColorType=WIN8
```


####Example 2:
This example will get current desktop background color, the red value of the highlight color, and menu background color.

```ini
[mBackground]
Measure=Plugin
Plugin=SysColor

[mHighlight]
Measure=Plugin
Plugin=SysColor
DisplayType=Red
ColorType=Highlight

[mMenuBackground]
Measure=Plugin
Plugin=SysColor
DisplayType=ALL
ColorType=Menu
```

####Example 3:
This example will get the current Aero color, and make it the background of the skin.

```ini
[mBackground]
Measure=Plugin
Plugin=SysColor
ColorType=Aero

[BackgroundMeter]
Meter=Image
SolidColor=[mBackground]
W=#CURRENTCONFIGWIDTH#
H=#CURRENTCONFIGHEIGHT#
DynamicVariables=1
```