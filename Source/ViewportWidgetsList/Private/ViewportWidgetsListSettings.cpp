// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListSettings.h"

using namespace ViewportWidgetsListSettings;
using namespace std;

namespace ViewportWidgetsListSettings
{

    // 文字列を区切り文字で分割する
    static vector<string> split(const string& str, char delimiter)
    {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter))
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    // FString → std::string の変換
    void ConvertFStringToStdString()
    {
        FString UnrealString = TEXT("Hello, Unreal Engine!");
        std::string StandardString = TCHAR_TO_UTF8(*UnrealString);

        // 結果を出力
        UE_LOG(LogTemp, Log, TEXT("FString: %s"), *UnrealString);
        UE_LOG(LogTemp, Log, TEXT("std::string: %s"), *FString(StandardString.c_str()));
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
        vector<string> Parts = split(path, '.');
        shared_ptr<Node> Current = shared_from_this();
        for (const string& Part : Parts)
        {
            Current = Current->GetChild(Part);
        }
        return Current;
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
        WidgetsSettings.DefaultTooltipText = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_TooltipText", "Open {0}"), DisplayName);
    }
}

void UViewportWidgetsListSettings::PostInitProperties()
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
    Super::PostInitProperties();
}

#if WITH_EDITOR
void UViewportWidgetsListSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListSettings, ViewportWidgetsListMenuProvidedWidgets))
    {
        FixDefaultTexts();
    }
}
#endif // WITH_EDITOR
