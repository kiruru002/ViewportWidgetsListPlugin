// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "Components/Widget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "ViewportWidgetsListSettings.generated.h"

UCLASS(config = EditorPerProjectUserSettings)
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListUserSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings", meta = (ConfigRestartRequired = true))
	bool bEnableViewportWidgetsListPlugin;

public:
	UViewportWidgetsListUserSettings();
};

UCLASS(config = ViewportWidgetsListSettings, DefaultConfig)
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListSettings : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings", meta = (ConfigRestartRequired = true))
	TArray<TSoftObjectPtr<UEditorUtilityWidgetBlueprint>> ViewportWidgetsListMenuProvidedWidgets;

public:
	UViewportWidgetsListSettings();
};
