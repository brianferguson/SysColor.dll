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

* Retrieval of the current Windows Aero color (including alpha transparency) for Windows Vista and above.
* 30 different non-Aero colors available (eg. `Background`, `Highlight`, `Menu`).
* Different display modes.
* Output in hex or decimal form.
* A numeric return of "1" means the color was retrieved. A numeric value of "-1" means the color was *not* retrieved. The numeric value can be retrieved through [section variables](http://docs.rainmeter.net/manual-beta/variables/section-variables) (eg. [MeasureName:]).

#####Note:
When using a Aero theme (for non-XP users), the only available color is the current Aero color. You can retrieve the other colors, however they may *not* be the current color shown. I am looking for ways to retrieve the different colors for the Aero theme.


Options
-
It is important to use [DynamicVariables](http://docs.rainmeter.net/manual/variables#DynamicVariables) with this plugin.

* **Hex** - When set to "1", the output color is in hexidecimal form. When set to "0", the output color is in decimal form separated by commas. `Hex=0` is default.
* **DisplayType** - Type of output. `DisplayType=All` is default. Options include:
  * **Red** - Output only the red value.
  * **Green** - Output only the green value.
  * **Blue** - Output only the blue value.
  * **Alpha** - Output only the alpha value (for `ColorType=Aero` only).
  * **RGB** - Output only the red, green and blue values (No alpha is output).
  * **ALL** - Output only the all values.
* **ColorType** - Type of color to retrieve. `ColorType=Desktop` is default. Options include:
  * **Aero** - Current color of Aero theme (including alpha transparency). This is for Windows Vista and above.
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

 #####Note:
 When using a Aero theme, some of the color types will not be accurately represented.

Changes
-
Here is a list of the major changes to the plugin.

#####Version:
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
This plugin was written in c++ using the [Rainmeter Plugin SDK](https://github.com/rainmeter/rainmeter-plugin-sdk)

To build this source code, you will need Visual Studio 2010. If you are using any _**paid**_ version of Visual Studio (Professional, Premium, Ultimate, etc.) you can open "PluginClipboard.sln" and build from there. If you are using Visual C++ 2010 Express, then please install the following **in order**:

1. Visual Studio 2010 Express Edition
2. Visual Studio 2010 SP1
3. Windows 7.1 SDK
4. KB2519277 compiler update

Then you can open "PluginClipboard.sln" and build from there.


Examples
-

####Example 1:
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

####Example 2:
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