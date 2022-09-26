[Variables]
; I wouldn't modify these settings unless you want to manually calculate the skins dimensions below
CellWidth=210
CellHeight=20
Padding=4

BackgroundColor=171720

; 20 rows (40 total) - 3 columns each (total of 6)
SkinWidth=((6 * #CellWidth#) + (17.5 * #Padding#))
SkinHeight=((20 * #CellHeight#) + 54 + (38 * #Padding#))

InfoX=([DWM_AFTERGLOW_BALANCE_Value:XW] + (#Padding# * 2))
InfoY=([DWM_AFTERGLOW_BALANCE_Value:Y] + #Padding#)
InfoW=((#CellWidth# * 2) - (#CellWidth# / 6) - (#Padding# * 3))
InfoH=((#CellHeight# * 2) + (#Padding# * 4))

; Styles

[Cell]
W=#CellWidth#
H=#CellHeight#
AntiAlias=1
Padding=#Padding#,#Padding#,#Padding#,#Padding#
DynamicVariables=1

[CellShortWidth]
W=(#CellWidth# / 6)

[CellLabel]
FontSize=12
SolidColor=#CellBackgroundColor#
FontColor=#CellForegroundColor#

[CellLabelLongName]
FontSize=8

[FirstSetColumnLabel]
StringAlign=RightCenter
X=((#Padding# * 3) + #CellWidth#)
Y=((2 * #Padding#) + (0.5 * #CellHeight#))

[SecondSetColumnLabel]
StringAlign=RightCenter
X=((#SkinWidth# / 2) + (#Padding# * 3) + #CellWidth# - 1)
Y=((2 * #Padding#) + (0.5 * #CellHeight#))

[ColumnValue]
StringAlign=LeftCenter
X=(#Padding# / 2)r
Y=r

[ColumnColor]
X=(#Padding# /  2)R
Y=(0 - (#CellHeight# / 2) - #Padding#)r

[NextRow]
Y=(#Padding# * 4)R

[Toggles]
X=(#InfoX#)
Y=([DWM_OPAQUE_BLEND_Value:Y] + (#Padding# * 3) + #CellHeight#)
W=""
H=""
FontSize=9
SolidColor=0,0,0,1
StringAlign=LeftBottom

[Links]
MouseOverAction=[!SetOption #CURRENTSECTION# InlineSetting "Underline"][!SetOption #CURRENTSECTION# InlineSetting2 "Color | [Hyperlink]"][!UpdateMeter #CURRENTSECTION#][!Redraw]
MouseLeaveAction=[!SetOption #CURRENTSECTION# InlineSetting ""][!SetOption #CURRENTSECTION# InlineSetting2 ""][!UpdateMeter #CURRENTSECTION#][!Redraw]

; Meters
[Background]
Meter=Shape
Shape=Rectangle 1,1,(#SkinWidth# - 2),(#SkinHeight# - 2) | Extend Colors
Shape2=Line (#SkinWidth# / 2),2, (#SkinWidth# / 2), (#SkinHeight# - 2) | Extend Colors
Colors=Fill Color #BackgroundColor# | Stroke Color [Accent]
DynamicVariables=1

[Accent_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel
Text=Accent

[Accent_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Accent

[Accent_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Accent]

[AERO_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=AERO

[AERO_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=AERO

[AERO_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[AERO]

[Desktop_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=Desktop

[Desktop_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Desktop

[Desktop_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Desktop]

[Window_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=Window

[Window_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Window

[Window_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Window]

[WindowFrame_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=WindowFrame

[WindowFrame_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=WindowFrame

[WindowFrame_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[WindowFrame]

[WindowText_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=WindowText

[WindowText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=WindowText

[WindowText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[WindowText]

[ActiveCaption_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ActiveCaption

[ActiveCaption_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ActiveCaption

[ActiveCaption_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ActiveCaption]

[ActiveCaptionGradient_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ActiveCaptionGradient

[ActiveCaptionGradient_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ActiveCaptionGradient

[ActiveCaptionGradient_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ActiveCaptionGradient]

[InactiveCaption_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=InactiveCaption

[InactiveCaption_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=InactiveCaption

[InactiveCaption_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[InactiveCaption]

[InactiveCaptionGradient_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=InactiveCaptionGradient

[InactiveCaptionGradient_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=InactiveCaptionGradient

[InactiveCaptionGradient_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[InactiveCaptionGradient]

[CaptionText_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=CaptionText

[CaptionText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=CaptionText

[Caption_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[CaptionText]

[InactiveCaptionText_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=InactiveCaptionText

[InactiveCaptionText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=InactiveCaptionText

[InactiveCaptionText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[InactiveCaptionText]

[ActiveBorder_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ActiveBorder

[ActiveBorder_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ActiveBorder

[ActiveBorder_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ActiveBorder]

[InActiveBorder_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=InActiveBorder

[InActiveBorder_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=InActiveBorder

[InActiveBorder_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[InActiveBorder]

[Highlight_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=Highlight

[Highlight_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Highlight

[Highlight_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Highlight]

[HighlightText_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=HighlightText

[HighlightText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=HighlightText

[HighlightText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[HighlightText]

[ButtonFace_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ButtonFace

[ButtonFace_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ButtonFace

[ButtonFace_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ButtonFace]

[ButtonText_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ButtonText

[ButtonText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ButtonText

[ButtonText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ButtonText]

[ButtonHighlight_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ButtonHighlight

[ButtonHighlight_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ButtonHighlight

[ButtonHighlight_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ButtonHighlight]

[ButtonShadow_Label]
Meter=String
MeterStyle=Cell | CellLabel | FirstSetColumnLabel | NextRow
Text=ButtonShadow

[ButtonShadow_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ButtonShadow

[ButtonShadow_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ButtonShadow]

; Second set of values

[Menu_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel
Text=Menu

[Menu_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Menu

[Menu_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Menu]

[MenuText_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=MenuText

[MenuText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=MenuText

[MenuText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[MenuText]

[MenuBar_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=MenuBar

[MenuBar_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=MenuBar

[MenuBar_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[MenuBar]

[MenuHighlight_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=MenuHighlight

[MenuHightlight_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=MenuHighlight

[MenuHighlight_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[MenuHighlight]

[3DDarkShadow_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=3DDarkShadow

[3DDarkShadow_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=3DDarkShadow

[3DDarkShadow_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[3DDarkShadow]

[3DLight_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=3DLight

[3DLight_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=3DLight

[3DLight_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[3DLight]

[GrayText_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=GrayText

[GrayText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=GrayText

[GrayText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[GrayText]

[ToolTipBackground_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=ToolTipBackground

[ToolTipBackground_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ToolTipBackground

[ToolTipBackground_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ToolTipBackground]

[ToolTipText_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=ToolTipText

[ToolTipText_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=ToolTipText

[ToolTipText_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[ToolTipText]

[AppWorkspace_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=AppWorkspace

[AppWorkspace_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=AppWorkspace

[AppWorkSpace_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[AppWorkspace]

[Scollbar_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=Scrollbar

[Scrollbar_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Scrollbar

[Scrollbar_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Scrollbar]

[Hyperlink_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=Hyperlink

[Hyperlink_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=Hyperlink

[Hyperlink_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[Hyperlink]

[WIN8_Label]
Meter=String
MeterStyle=Cell | CellLabel | SecondSetColumnLabel | NextRow
Text=WIN8

[WIN8_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=WIN8

[WIN8_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[WIN8]

[DWM_COLOR_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_COLOR

[DWM_COLOR_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=DWM_COLOR

[DWM_COLOR_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[DWM_COLOR]

[DWM_AFTERGLOW_COLOR_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_AFTERGLOW_COLOR

[DWM_AFTERGLOW_COLOR_Value]
Meter=String
MeterStyle=Cell | CellLabel | ColumnValue
MeasureName=DWM_AFTERGLOW_COLOR

[DWM_AFTERGLOW_COLOR_Color]
Meter=Image
MeterStyle=Cell | ColumnColor
SolidColor=[DWM_AFTERGLOW_COLOR]

[DWM_COLOR_BALANCE_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_COLOR_BALANCE

[DWM_COLOR_BALANCE_Value]
Meter=String
MeterStyle=Cell | CellShortWidth | CellLabel | ColumnValue
MeasureName=DWM_COLOR_BALANCE

[DWM_COLOR_BALANCE_Color]
Meter=Image
MeterStyle=Cell | ColumnColor

[DWM_AFTERGLOW_BALANCE_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_AFTERGLOW_BALANCE

[DWM_AFTERGLOW_BALANCE_Value]
Meter=String
MeterStyle=Cell | CellShortWidth | CellLabel | ColumnValue
MeasureName=DWM_AFTERGLOW_BALANCE

[DWM_AFTERGLOW_BALANCE_Color]
Meter=Image
MeterStyle=Cell | ColumnColor

[DWM_BLUR_BALANCE_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_BLUR_BALANCE

[DWM_BLUR_BALANCE_Value]
Meter=String
MeterStyle=Cell | CellShortWidth | CellLabel | ColumnValue
MeasureName=DWM_BLUR_BALANCE

[DWM_BLUR_BALANCE_Color]
Meter=Image
MeterStyle=Cell | ColumnColor

[DWM_GLASS_REFLECTION_INTENSITY_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_GLASS_REFLECTION_INTENSITY

[DWM_GLASS_REFLECTION_INTENSITY_Value]
Meter=String
MeterStyle=Cell | CellShortWidth | CellLabel | ColumnValue
MeasureName=DWM_GLASS_REFLECTION_INTENSITY

[DWM_GLASS_REFLECTION_INTENSITY_Color]
Meter=Image
MeterStyle=Cell | ColumnColor

[DWM_OPAQUE_BLEND_Label]
Meter=String
MeterStyle=Cell | CellLabel | CellLabelLongName | SecondSetColumnLabel | NextRow
Text=DWM_OPAQUE_BLEND

[DWM_OPAQUE_BLEND_Value]
Meter=String
MeterStyle=Cell | CellShortWidth | CellLabel | ColumnValue
MeasureName=DWM_OPAQUE_BLEND

[DWM_OPAQUE_BLEND_Color]
Meter=Image
MeterStyle=Cell | ColumnColor

[Information]
Meter=String
MeterStyle=Cell | CellLabel
SolidColor=#BackgroundColor#
Padding=5,5,5,5
Text=SysColor v2.0#CRLF#by: brianferguson
InlineSetting=Size | 30
InlinePattern=^(.*) v
InlineSetting2=Size | 9
InlinePattern2=SysColor(.*)#CRLF#
InlineSetting3=CharacterSpacing | 1.25,1
InlinePattern3=v(.*)\.
InlineSetting4=Weight | 800
InlinePattern4=v(.*)#CRLF#
InlineSetting5=Color | [Accent]
InlinePattern5=v(.*)#CRLF#
InlineSetting6=Size | 10
InlinePattern6=#CRLF#(.*)$
StringAlign=CenterCenter
X=(#InfoX# + #Padding# + (#InfoW# / 2))
Y=(#InfoY# + #Padding# + (#InfoH# / 2))
W=#InfoW#
H=#InfoH#

[Hex]
Meter=String
MeterStyle=Cell | CellLabel | Toggles | Links
Text=[ HEX ]
LeftMouseUpAction=[!SetVariable Hex "1"][!UpdateMeasure *][!UpdateMeter *][!Redraw]

[Dec]
Meter=String
MeterStyle=Cell | CellLabel | Toggles | Links
X=5R
Text=[ DEC ]
LeftMouseUpAction=[!SetVariable Hex "0"][!UpdateMeasure *][!UpdateMeter *][!Redraw]

[Link]
Meter=String
MeterStyle=Cell | CellLabel | Links
SolidColor=0,0,0,1
FontSize=9
X=(#SkinWidth# - #Padding#)
Y=(#SkinHeight# - 22)
W=""
H=""
StringAlign=Right
Text=https://github.com/brianferguson/SysColor.dll
LeftMouseUpAction=[https://github.com/brianferguson/SysColor.dll]
