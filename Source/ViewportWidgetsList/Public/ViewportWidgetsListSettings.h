// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "Components/Widget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Subsystems/EngineSubsystem.h"
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

    // サブメニューの名前。ピリオドを使用して階層を表現できます。(例：path.to.item)
    // Name of the sub-menu. Hierarchies can be represented using periods. (ex: path.to.item)
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


UCLASS()
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListUserSettingsSubsystem : public UEngineSubsystem
{
    GENERATED_BODY()
public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSettingsBoolValueChanged, bool, NewValue);

public:

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnSettingsBoolValueChanged OnIncludeHandlerWidgetHierarchyClickedChanged;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnSettingsBoolValueChanged OnIncludeAllUserWidgetsHierarchyClickedChanged;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnSettingsBoolValueChanged OnIncludeHandlerWidgetHierarchyHoveredChanged;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnSettingsBoolValueChanged OnIncludeAllUserWidgetsHierarchyHoveredChanged;

};

UCLASS(config = EditorPerProjectUserSettings)
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListUserSettings : public UObject
{
    GENERATED_BODY()
public:
    // ビューポートウィジェットリストプラグインを有効にする
    // Enable the viewport widgets list plugin
    UPROPERTY(EditAnywhere, BlueprintReadOnly, config, Category = "UViewportWidgetsListUserSettings", meta = (ConfigRestartRequired = true))
    bool bEnableViewportWidgetsListPlugin;

    // トリガーとなったウィジェットの階層を表示する (Clicked)
    // Display the hierarchy of the widget that triggered the event (Clicked)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, config, Category = "UViewportWidgetsListUserSettings", meta = (DisplayName = "Include Handler Widget Hierarchy (Clicked)"))
    bool bIncludeHandlerWidgetHierarchyClicked;

    // ウィジェットからたどれる親の階層を全部表示する (Clicked)
    // Display all the parent hierarchies that can be traced from the widget. (Clicked)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, config, Category = "UViewportWidgetsListUserSettings", meta = (DisplayName = "Include All User Widget Hierarchy (Clicked)"))
    bool bIncludeAllUserWidgetsHierarchyClicked;

    // トリガーとなったウィジェットの階層を表示する (Hovered)
    // Display the hierarchy of the widget that triggered the event (Hovered)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, config, Category = "UViewportWidgetsListUserSettings", meta = (DisplayName = "Include Handler Widget Hierarchy (Hovered)"))
    bool bIncludeHandlerWidgetHierarchyHovered;

    // ウィジェットからたどれる親の階層を全部表示する (Hovered)
    // Display all the parent hierarchies that can be traced from the widget. (Hovered)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, config, Category = "UViewportWidgetsListUserSettings", meta = (DisplayName = "Include All User Widget Hierarchy (Hovered)"))
    bool bIncludeAllUserWidgetsHierarchyHovered;

    // 追加されたカスタムサブメニューの各項目はマウスホバーするだけで開く
    // Each item in the added custom submenu will open when you hover over it with your mouse.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, config, Category = "UViewportWidgetsListUserSettings")
    bool bOpenCustomSubmenuOnMouseHover;

public:
    UViewportWidgetsListUserSettings();

    UFUNCTION(BlueprintPure = false, Category = "UViewportWidgetsListUserSettings")
    static const UViewportWidgetsListUserSettings* GetViewportWidgetsListUserSettings();

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    UFUNCTION(BlueprintCallable)
    void SetIncludeHandlerWidgetHierarchyClicked(bool NewValue);

    UFUNCTION(BlueprintCallable)
    void SetIncludeAllUserWidgetsHierarchyClicked(bool NewValue);

    UFUNCTION(BlueprintCallable)
    void SetIncludeHandlerWidgetHierarchyHovered(bool NewValue);

    UFUNCTION(BlueprintCallable)
    void SetIncludeAllUserWidgetsHierarchyHovered(bool NewValue);
#endif
};

UCLASS(config = ViewportWidgetsListSettings, DefaultConfig)
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListSettings : public UObject
{
    GENERATED_BODY()
public:
    // 上部メニューに追加する項目の名前
    // Name of the viewport widgets list menu
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText ViewportWidgetsListMenuName;

    // 上部メニューに追加する項目の説明文
    // Description of the viewport widgets list menu
    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    FText ViewportWidgetsListMenuTooltip;

    UPROPERTY(EditAnywhere, config, Category = "UViewportWidgetsListSettings")
    TArray<FViewportWidgetsListSettingsEntry> ViewportWidgetsListMenuProvidedWidgets;

    std::shared_ptr<ViewportWidgetsListSettings::Node> HierarchyRoot;

public:
    UViewportWidgetsListSettings();
    void FixDefaultTexts();
    void UpdateMenuEntryNodes();
    virtual void PostInitProperties() override;
#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
