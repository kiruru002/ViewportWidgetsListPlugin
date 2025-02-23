// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "ViewportWidgetsListFunctionLibrary.generated.h"

UCLASS()
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListFunctionLibrary")
    static bool WidgetIsChildOf(UWidget* Widget, UWidget* ParentWidget);

    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListFunctionLibrary")
    static bool WidgetIsIncludedIn(UWidget* Widget, UUserWidget* UserWidget);

    /**
    *   Retrieves widgets hierarchy from the starting widget.
    */
    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListFunctionLibrary")
    static void GetWidgetHierarchyList(UWidget* StartingWidget, UWidget* StopWidget, TSubclassOf<UWidget> FindType, TArray<UWidget*>& List);

    /**
    * Finds the first parent widget of the given type and returns it, or null if no parent could be found.
    */
    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListFunctionLibrary", meta = (DeterminesOutputType = Type))
    static UWidget* FindParentWidgetOfType(UWidget* StartingWidget, TSubclassOf<UWidget> Type);

    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListFunctionLibrary")
    static bool IsViewportWidgetsListPluginEnabled();

};
