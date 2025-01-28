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
        vector<string> parts = split(path, '.');
        shared_ptr<Node> current = shared_from_this();
        for (const string& part : parts)
        {
            current = current->GetChild(part);
        }
        return current;
    }
}

FViewportWidgetsListSettingsEntry::FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint> Widget, FName SubMenuName, FName SectionName, FText Label, FText ToolTipText, FText DefaultLabel, FText DefaultToolTipText)
    : Widget(Widget)
    , SubMenuName(SubMenuName)
    , SectionName(SectionName)
    , Label(Label)
    , ToolTipText(ToolTipText)
    , DefaultLabel(DefaultLabel)
    , DefaultToolTipText(DefaultToolTipText)
{

}

UViewportWidgetsListUserSettings::UViewportWidgetsListUserSettings()
    : Super()
    , bEnableViewportWidgetsListPlugin(true)
{

}

UViewportWidgetsListSettings::UViewportWidgetsListSettings()
    : Super()
    , ViewportWidgetsListMenuProvidedWidgets({
    FViewportWidgetsListSettingsEntry(TSoftObjectPtr<UEditorUtilityWidgetBlueprint>(FSoftObjectPath(TEXT("/ViewportWidgetsList/EUW_ViewportWidgetsList.EUW_ViewportWidgetsList"))), NAME_None, NAME_None, FText::FromString(TEXT("")), FText::FromString(TEXT("")), FText::FromString(TEXT("EUW_ViewportWidgetsList")), FText::FromString(TEXT("Open EUW_ViewportWidgetsList.")))
        })
{
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
    Super::PostInitProperties();
}

#if WITH_EDITOR
void UViewportWidgetsListSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.GetMemberPropertyName() == GET_MEMBER_NAME_CHECKED(UViewportWidgetsListSettings, ViewportWidgetsListMenuProvidedWidgets))
    {
        for (FViewportWidgetsListSettingsEntry& WidgetsSettings : ViewportWidgetsListMenuProvidedWidgets)
        {
            const FText DisplayName = FText::FromString(FPaths::GetBaseFilename(WidgetsSettings.Widget.ToSoftObjectPath().GetAssetPath().ToString()));
            WidgetsSettings.DefaultLabel = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_Label", "{0}"), DisplayName);
            WidgetsSettings.DefaultToolTipText = FText::Format(NSLOCTEXT("FViewportWidgetsListModule", "OpenEUW_ToolTipText", "Open {0}."), DisplayName);
        }
    }
}
#endif // WITH_EDITOR
