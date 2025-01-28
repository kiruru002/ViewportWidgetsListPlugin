// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "Components/Widget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "ViewportWidgetsListSettings.generated.h"

USTRUCT()
struct VIEWPORTWIDGETSLIST_API FViewportWidgetsListSettingsEntry
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget;

    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FName SectionName;

    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText Label;

    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText ToolTipText;

    UPROPERTY(VisibleAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText DefaultLabel;

    UPROPERTY(VisibleAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText DefaultToolTipText;

public:
    FViewportWidgetsListSettingsEntry() = default;
    FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget, FName SectionName, FText Label, FText ToolTipText, FText DefaultLabel, FText DefaultToolTipText);
};

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
    TArray<FViewportWidgetsListSettingsEntry> ViewportWidgetsListMenuProvidedWidgets;

public:
    UViewportWidgetsListSettings();

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
