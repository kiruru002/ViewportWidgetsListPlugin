// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ViewportWidgetsListSettings.generated.h"

UCLASS(config = EditorPerProjectUserSettings)
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings", meta = (ConfigRestartRequired = true))
	bool bEnableViewportWidgetsListPlugin;

public:
	UViewportWidgetsListSettings();
};
