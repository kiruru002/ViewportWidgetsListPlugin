// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListFunctionLibrary.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

UWidget* UViewportWidgetsListFunctionLibrary::FindParentWidgetOfType(UWidget* StartingWidget, TSubclassOf<UWidget> Type)
{
    if (IsValid(StartingWidget) && IsValid(StartingWidget->GetParent()))
    {
        UWidget* ParentWidget = Cast<UWidget>(StartingWidget->GetParent());
        if (IsValid(ParentWidget))
        {
            if (ParentWidget->GetClass()->IsChildOf(Type))
            {
                return ParentWidget;
            }
            return FindParentWidgetOfType(ParentWidget, Type);
        }
        UObject* OuterObject = StartingWidget->GetOuter();
        if (IsValid(OuterObject))
        {
            UWidgetTree* WidgetTree = Cast<UWidgetTree>(OuterObject);
            if (WidgetTree == nullptr)
            {
                return nullptr;
            }
            if (IsValid(OuterObject->GetOuter()))
            {
                UWidget* ParentUserWidget = Cast<UUserWidget>(OuterObject->GetOuter());
                if (IsValid(ParentUserWidget) && ParentUserWidget->GetClass()->IsChildOf(Type))
                {
                    return ParentUserWidget;
                }
                return FindParentWidgetOfType(ParentUserWidget, Type);
            }
        }
    }
    return nullptr;
}

UViewportWidgetsListSettings* UViewportWidgetsListFunctionLibrary::GetMutableDefaultViewportWidgetsListSettings()
{
    return GetMutableDefault<UViewportWidgetsListSettings>();
}

bool UViewportWidgetsListFunctionLibrary::IsViewportWidgetsListPluginEnabled()
{
    const UViewportWidgetsListSettings* PluginSettings = GetDefault<UViewportWidgetsListSettings>();
    return PluginSettings->bEnableViewportWidgetsListPlugin;
}
