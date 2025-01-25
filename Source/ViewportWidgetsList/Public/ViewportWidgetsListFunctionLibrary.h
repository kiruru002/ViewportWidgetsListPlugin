// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViewportWidgetsListFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListFunctionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:

    /**
    * Finds the first parent widget of the given type and returns it, or null if no parent could be found.
    */
    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListFunctionLibrary", meta = (DeterminesOutputType = Type))
    static UWidget* FindParentWidgetOfType(UWidget* StartingWidget, TSubclassOf<UWidget> Type);

};
