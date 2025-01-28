// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListSettings.h"

using namespace ViewportWidgetsListSettings;
using namespace std;

namespace ViewportWidgetsListSettings
{

    // ï∂éöóÒÇãÊêÿÇËï∂éöÇ≈ï™äÑÇ∑ÇÈ
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

    // FString Å® std::string ÇÃïœä∑
    void ConvertFStringToStdString()
    {
        FString UnrealString = TEXT("Hello, Unreal Engine!");
        std::string StandardString = TCHAR_TO_UTF8(*UnrealString);

        // åãâ ÇèoóÕ
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
{

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
        WidgetsSettings.DefaultTooltipText = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_TooltipText", "Open {0}."), DisplayName);
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
