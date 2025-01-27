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
    while (IsValid(StartingWidget) && StartingWidget->GetFName().IsValid())
    {
        UWidget* LocalRoot = StartingWidget;
        UWidget* LocalParent = LocalRoot->GetParent();
        while (IsValid(LocalParent))
        {
            if (LocalParent->IsA(Type))
            {
                return LocalParent;
            }
            LocalRoot = LocalParent;
            LocalParent = LocalParent->GetParent();
        }

        if (IsValid(LocalRoot->GetOuter()))
        {
            UWidgetTree* WidgetTree = Cast<UWidgetTree>(LocalRoot->GetOuter());
            if (WidgetTree == nullptr)
            {
                break;
            }

            if (IsValid(WidgetTree->GetOuter()))
            {
                StartingWidget = Cast<UUserWidget>(WidgetTree->GetOuter());
                if (StartingWidget && StartingWidget->IsA(Type))
                {
                    return StartingWidget;
                }
            }
        }
        StartingWidget = nullptr;
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
