// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListFunctionLibrary.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetTree.h"

UWidget* UViewportWidgetsListFunctionLibrary::FindParentWidgetOfType(UWidget* StartingWidget, TSubclassOf<UWidget> Type)
{
    while (StartingWidget)
    {
        UWidget* LocalRoot = StartingWidget;
        UWidget* LocalParent = Cast<UWidget>(LocalRoot->GetParent());
        while (LocalParent)
        {
            if (LocalParent->IsA(Type))
            {
                return LocalParent;
            }
            LocalRoot = LocalParent;
            LocalParent = LocalParent->GetParent();
        }
        UWidgetTree* WidgetTree = Cast<UWidgetTree>(LocalRoot->GetOuter());
        if (WidgetTree == nullptr)
        {
            break;
        }
        StartingWidget = Cast<UUserWidget>(WidgetTree->GetOuter());
        if (StartingWidget && StartingWidget->IsA(Type))
        {
            return StartingWidget;
        }
    }
    return nullptr;
}
