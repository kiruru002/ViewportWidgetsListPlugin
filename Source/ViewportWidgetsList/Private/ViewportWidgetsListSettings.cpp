// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListSettings.h"

UViewportWidgetsListUserSettings::UViewportWidgetsListUserSettings()
    : Super()
    , bEnableViewportWidgetsListPlugin(true)
{

}

UViewportWidgetsListSettings::UViewportWidgetsListSettings()
    : Super()
    , ViewportWidgetsListMenuProvidedWidgets({
    TSoftObjectPtr<UEditorUtilityWidgetBlueprint>(FSoftObjectPath(TEXT("/ViewportWidgetsList/EUW_ViewportWidgetsList.EUW_ViewportWidgetsList")))
        })
{
}
