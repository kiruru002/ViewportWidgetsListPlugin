// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListSettings.h"

FViewportWidgetsListSettingsEntry::FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget, FName SubMenuName, FName SectionName, FText Label, FText ToolTipText, FText DefaultLabel, FText DefaultToolTipText)
    : Widget(Widget)
    , SubMenuName(SubMenuName)
    , SectionName(SectionName)
    , Label(Label)
    , ToolTipText(ToolTipText)
    , DefaultLabel(DefaultLabel)
    , DefaultToolTipText(DefaultToolTipText)
{

}

UViewportWidgetsListUserSettings::UViewportWidgetsListUserSettings()
    : Super()
    , bEnableViewportWidgetsListPlugin(true)
{

}

UViewportWidgetsListSettings::UViewportWidgetsListSettings()
    : Super()
    , ViewportWidgetsListMenuProvidedWidgets({
    FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint>(FSoftObjectPath(TEXT("/ViewportWidgetsList/EUW_ViewportWidgetsList.EUW_ViewportWidgetsList"))), NAME_None, NAME_None, FText::FromString(TEXT("")), FText::FromString(TEXT("")), FText::FromString(TEXT("EUW_ViewportWidgetsList")), FText::FromString(TEXT("Open EUW_ViewportWidgetsList.")))
        })
{
}

#if WITH_EDITOR
void UViewportWidgetsListSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListSettings, ViewportWidgetsListMenuProvidedWidgets))
    {
        for (FViewportWidgetsListSettingsEntry& WidgetsSettings : ViewportWidgetsListMenuProvidedWidgets)
        {
            const FText DisplayName = FText::FromString(FPaths::GetBaseFilename(WidgetsSettings.Widget.ToSoftObjectPath().GetAssetPathName().ToString()));
            WidgetsSettings.DefaultLabel = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_Label", "{0}"), DisplayName);
            WidgetsSettings.DefaultToolTipText = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_ToolTipText", "Open {0}."), DisplayName);
        }
    }
}
#endif // WITH_EDITOR
