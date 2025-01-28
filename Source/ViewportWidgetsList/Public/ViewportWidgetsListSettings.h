// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "Components/Widget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include <map>
#include <memory>
#include <string>
#include "ViewportWidgetsListSettings.generated.h"

USTRUCT()
struct VIEWPORTWIDGETSLIST_API FViewportWidgetsListSettingsEntry
{
    GENERATED_BODY()
public:

    // ウィジェットのソフト参照
    // Soft object reference to the widget
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget;

    // サブメニューの名前。コンマやピリオドを使用して階層を表現できます。
    // Name of the sub-menu. Hierarchies can be represented using commas or periods.
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FName SubMenuName;

    // メニュー項目セクションの名前
    // Name of the menu item
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FName SectionName;

    // メニュー項目のテキスト
    // Label of the menu item
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText Label;

    // メニュー項目のツールチップ
    // Tooltip of the menu item
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText TooltipText;

    // メニュー項目のテキスト (デフォルト)
    // Label of the menu item (Default)
    UPROPERTY(VisibleAnywhere, Category = "UViewportWidgetsListSettings")
    FText DefaultLabel;

    // メニュー項目のツールチップ (デフォルト)
    // Tooltip of the menu item (Default)
    UPROPERTY(VisibleAnywhere, Category = "UViewportWidgetsListSettings")
    FText DefaultTooltipText;

public:
    FViewportWidgetsListSettingsEntry() = default;
    FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget, FName SubMenuName, FName SectionName, FText Label, FText TooltipText, FText DefaultLabel, FText DefaultToolTipText);
};

namespace ViewportWidgetsListSettings
{
    class Node : public std::enable_shared_from_this<Node>
    {
    public:
        TArray<const FViewportWidgetsListSettingsEntry*> Entries;
        // 子
        std::map<std::string, std::shared_ptr<Node>> Children;
        // 子を取得（存在しない場合は作成）
        std::shared_ptr<Node> GetChild(const std::string& name);
        std::shared_ptr<Node> GetChildPath(const std::string& path);
    };
}

UCLASS(config = EditorPerProjectUserSettings)
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListUserSettings : public UObject
{
    GENERATED_BODY()
public:
    // ビューポートウィジェットリストプラグインを有効にする
    // Enable the viewport widgets list plugin
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
    // 上部メニューに追加する項目の名前
    // Name of the viewport widgets list menu
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings", meta = (ConfigRestartRequired = true))
    FText ViewportWidgetsListMenuName;

    // 上部メニューに追加する項目の説明文
    // Description of the viewport widgets list menu
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings", meta = (ConfigRestartRequired = true))
    FText ViewportWidgetsListMenuTooltip;

    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings", meta = (ConfigRestartRequired = true))
    TArray<FViewportWidgetsListSettingsEntry> ViewportWidgetsListMenuProvidedWidgets;

    std::shared_ptr<ViewportWidgetsListSettings::Node> HierarchyRoot;

public:
    UViewportWidgetsListSettings();
    void FixDefaultTexts();
    virtual void PostInitProperties() override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
