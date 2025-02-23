// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListSettings.h"
#include "ViewportWidgetsList.h"
#include "Modules/ModuleManager.h"
#include <string>
#include <vector>
#include <memory>

using namespace ViewportWidgetsListSettings;
using namespace std;

namespace ViewportWidgetsListSettings
{
    // 文字列を区切り文字で分割する
    // ただし、二重引用符で囲まれた文字列は区切り文字として扱わない
    // 例：("\"1.1\".A.B", '.') → ["1.1", "A", "B"]
    static std::vector<std::string> SplitPath(const std::string& str, char delimiter)
    {
        std::vector<std::string> tokens;
        bool inQuotes = false;
        bool inEscape = false;
        std::string token;

        for (char ch : str)
        {
            if (inEscape)
            {
                token.push_back(ch);
                inEscape = false;
            }
            else if (ch == '\\')
            {
                inEscape = true;
            }
            else if (ch == '"' || ch == '\'')
            {
                inQuotes = !inQuotes;
            }
            else if (ch == delimiter && !inQuotes)
            {
                tokens.push_back(token);
                token.clear();
            }
            else
            {
                token.push_back(ch);
            }
        }
        if (!token.empty())
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    std::shared_ptr<Node> Node::GetChild(const std::string& name)
    {
        if (Children.find(name) == Children.end())
        {
            Children[name] = std::make_shared<Node>();
        }
        return Children[name];
    }
    
    std::shared_ptr<Node> Node::GetChildPath(const std::string& path)
    {
        std::vector<std::string> parts = SplitPath(path, '.');
        std::shared_ptr<Node> current = shared_from_this();
        for (const std::string& part : parts)
        {
            current = current->GetChild(part);
            if (!current)
            {
                break;
            }
        }
        return current;
    }
}

FViewportWidgetsListSettingsEntry::FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget, FName SubMenuName, FName SectionName, FText Label, FText TooltipText, FText DefaultLabel, FText DefaultTooltipText)
    : Widget(Widget)
    , SubMenuName(SubMenuName)
    , SectionName(SectionName)
    , Label(Label)
    , TooltipText(TooltipText)
    , DefaultLabel(DefaultLabel)
    , DefaultTooltipText(DefaultTooltipText)
{

}

UViewportWidgetsListUserSettings::UViewportWidgetsListUserSettings()
    : Super()
    , bEnableViewportWidgetsListPlugin(true)
    , bIncludeHandlerWidgetHierarchyClicked(true)
    , bIncludeAllUserWidgetsHierarchyClicked(false)
    , bIncludeHandlerWidgetHierarchyHovered(true)
    , bIncludeAllUserWidgetsHierarchyHovered(false)
    , bOpenCustomSubmenuOnMouseHover(true)
{

}

const UViewportWidgetsListUserSettings* UViewportWidgetsListUserSettings::GetViewportWidgetsListUserSettings()
{
    return GetDefault<UViewportWidgetsListUserSettings>();
}

void UViewportWidgetsListUserSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (auto* Subsystem = GEngine->GetEngineSubsystem<UViewportWidgetsListUserSettingsSubsystem>())
    {
        FProperty* ChangedProperty = PropertyChangedEvent.Property;
        if (ChangedProperty)
        {
            if (ChangedProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListUserSettings, bIncludeHandlerWidgetHierarchyClicked))
            {
                Subsystem->OnIncludeHandlerWidgetHierarchyClickedChanged.Broadcast(bIncludeHandlerWidgetHierarchyClicked);
            }
            else if (ChangedProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListUserSettings, bIncludeAllUserWidgetsHierarchyClicked))
            {
                Subsystem->OnIncludeAllUserWidgetsHierarchyClickedChanged.Broadcast(bIncludeAllUserWidgetsHierarchyClicked);
            }
            else if (ChangedProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListUserSettings, bIncludeHandlerWidgetHierarchyHovered))
            {
                Subsystem->OnIncludeHandlerWidgetHierarchyHoveredChanged.Broadcast(bIncludeHandlerWidgetHierarchyHovered);
            }
            else if (ChangedProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListUserSettings, bIncludeAllUserWidgetsHierarchyHovered))
            {
                Subsystem->OnIncludeAllUserWidgetsHierarchyHoveredChanged.Broadcast(bIncludeAllUserWidgetsHierarchyHovered);
            }
            else if (ChangedProperty->GetFName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListUserSettings, bOpenCustomSubmenuOnMouseHover))
            {
                FModuleManager::GetModuleChecked<FViewportWidgetsListModule>(TEXT("ViewportWidgetsList")).RegisterMenus();
            }
        }
    }
}

void UViewportWidgetsListUserSettings::SetIncludeHandlerWidgetHierarchyClicked(bool NewValue)
{
    bIncludeHandlerWidgetHierarchyClicked = NewValue;
    SaveConfig();
    if (auto* Subsystem = GEngine->GetEngineSubsystem<UViewportWidgetsListUserSettingsSubsystem>())
    {
        Subsystem->OnIncludeHandlerWidgetHierarchyClickedChanged.Broadcast(bIncludeHandlerWidgetHierarchyClicked);
    }
}

void UViewportWidgetsListUserSettings::SetIncludeAllUserWidgetsHierarchyClicked(bool NewValue)
{
    bIncludeAllUserWidgetsHierarchyClicked = NewValue;
    SaveConfig();
    if (auto* Subsystem = GEngine->GetEngineSubsystem<UViewportWidgetsListUserSettingsSubsystem>())
    {
        Subsystem->OnIncludeAllUserWidgetsHierarchyClickedChanged.Broadcast(bIncludeAllUserWidgetsHierarchyClicked);
    }
}

void UViewportWidgetsListUserSettings::SetIncludeHandlerWidgetHierarchyHovered(bool NewValue)
{
    bIncludeHandlerWidgetHierarchyHovered = NewValue;
    SaveConfig();
    if (auto* Subsystem = GEngine->GetEngineSubsystem<UViewportWidgetsListUserSettingsSubsystem>())
    {
        Subsystem->OnIncludeHandlerWidgetHierarchyHoveredChanged.Broadcast(bIncludeHandlerWidgetHierarchyHovered);
    }
}

void UViewportWidgetsListUserSettings::SetIncludeAllUserWidgetsHierarchyHovered(bool NewValue)
{
    bIncludeAllUserWidgetsHierarchyHovered = NewValue;
    SaveConfig();
    if (auto* Subsystem = GEngine->GetEngineSubsystem<UViewportWidgetsListUserSettingsSubsystem>())
    {
        Subsystem->OnIncludeAllUserWidgetsHierarchyHoveredChanged.Broadcast(bIncludeAllUserWidgetsHierarchyHovered);
    }
}

UViewportWidgetsListSettings::UViewportWidgetsListSettings()
    : Super()
    , ViewportWidgetsListMenuName(NSLOCTEXT("FViewportWidgetsListModule", "FViewportWidgetsListModule_Menu_Label", "EUW"))
    , ViewportWidgetsListMenuTooltip(NSLOCTEXT("FViewportWidgetsListModule", "FViewportWidgetsListModule_Menu_Description", "EUW Menu"))
    , ViewportWidgetsListMenuProvidedWidgets({
    FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint>(FSoftObjectPath(TEXT("/ViewportWidgetsList/EUW_ViewportWidgetsList.EUW_ViewportWidgetsList"))), NAME_None, NAME_None, FText::GetEmpty(), FText::GetEmpty(), FText::GetEmpty(), FText::GetEmpty())
        })
{
}

void UViewportWidgetsListSettings::FixDefaultTexts()
{
    for (FViewportWidgetsListSettingsEntry& WidgetsSettings : ViewportWidgetsListMenuProvidedWidgets)
    {
        const FText DisplayName = FText::FromName(WidgetsSettings.Widget.ToSoftObjectPath().GetAssetPath().GetAssetName());
        WidgetsSettings.DefaultLabel = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_Label", "{0}"), DisplayName);
        WidgetsSettings.DefaultTooltipText = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_TooltipText", "{0}を開く\nOpen {0}"), DisplayName);
    }
}

void UViewportWidgetsListSettings::UpdateMenuEntryNodes()
{
    HierarchyRoot = make_shared<Node>();
    for (const FViewportWidgetsListSettingsEntry& Widget : ViewportWidgetsListMenuProvidedWidgets)
    {
        std::shared_ptr<Node> Hierarchy = nullptr;
        if (Widget.SubMenuName.IsNone())
        {
            Hierarchy = HierarchyRoot;
        }
        else
        {
            const std::string HierarchyString = TCHAR_TO_UTF8(*Widget.SubMenuName.ToString());
            Hierarchy = HierarchyRoot->GetChildPath(HierarchyString);
        }
        Hierarchy->Entries.Add(&Widget);
    }
    FixDefaultTexts();
}

void UViewportWidgetsListSettings::PostInitProperties()
{
    UpdateMenuEntryNodes();
    Super::PostInitProperties();
}

#if WITH_EDITOR
void UViewportWidgetsListSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListSettings, ViewportWidgetsListMenuName))
    {
        FModuleManager::GetModuleChecked<FViewportWidgetsListModule>(TEXT("ViewportWidgetsList")).RegisterMenus();
    }
    else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListSettings, ViewportWidgetsListMenuTooltip))
    {
        FModuleManager::GetModuleChecked<FViewportWidgetsListModule>(TEXT("ViewportWidgetsList")).RegisterMenus();
    }
    else if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListSettings, ViewportWidgetsListMenuProvidedWidgets))
    {
        UpdateMenuEntryNodes();
        FModuleManager::GetModuleChecked<FViewportWidgetsListModule>(TEXT("ViewportWidgetsList")).RegisterMenus();
    }
}
#endif // WITH_EDITOR
