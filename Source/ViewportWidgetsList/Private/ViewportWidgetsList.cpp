// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListFunctionLibrary.h"
#include "Modules/ModuleManager.h"
#include "Templates/SubclassOf.h"
#include "Components/Widget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"
#include "Editor.h"
#include "ISettingsModule.h"
#include "ToolMenus.h"
#include "ToolMenu.h"
#include "ToolMenuSection.h"

#define LOCTEXT_NAMESPACE "FViewportWidgetsListModule"

void FViewportWidgetsListModule::StartupModule()
{
#if WITH_EDITOR
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->RegisterSettings("Project", "Plugins", "ViewportWidgetsListUserSettings",
            LOCTEXT("ViewportWidgetsListUserSettings_Name", "UViewportWidgetsListSettings (User)"),
            LOCTEXT("ViewportWidgetsListUserSettings_Description", "ViewportWidgetsListPlugin's Settings"),
            GetMutableDefault<UViewportWidgetsListUserSettings>()
        );
        SettingsModule->RegisterSettings("Project", "Plugins", "ViewportWidgetsListSettings",
            LOCTEXT("ViewportWidgetsListSettings_Name", "UViewportWidgetsListSettings"),
            LOCTEXT("ViewportWidgetsListSettings_Description", "ViewportWidgetsListPlugin's Settings"),
            GetMutableDefault<UViewportWidgetsListSettings>()
        );
    }
    RegisterMenus();
#endif //WITH_EDITOR
}

void FViewportWidgetsListModule::ShutdownModule()
{
#if WITH_EDITOR
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->UnregisterSettings("Editor", "Plugins", "ViewportWidgetsListSettings");
    }
#endif //WITH_EDITOR
}

void FViewportWidgetsListModule::RegisterMenus()
{
    if (UViewportWidgetsListFunctionLibrary::IsViewportWidgetsListPluginEnabled())
    {
        // メニューシステムの初期化
        if (!UToolMenus::IsToolMenuUIEnabled())
        {
            return;
        }
        UToolMenus* ToolMenus = UToolMenus::Get();
        // メインメニューに項目を追加
        {
            UToolMenu* Menu = ToolMenus->ExtendMenu("Mainframe.MainMenu");
            if (Menu)
            {
                const UViewportWidgetsListSettings* PluginSettings = GetDefault<UViewportWidgetsListSettings>();
                UToolMenu* SubMenu = Menu->AddSubMenu(Menu,
                    NAME_None,
                    TEXT("FViewportWidgetsListModule_Menu"),
                    PluginSettings->ViewportWidgetsListMenuName,
                    PluginSettings->ViewportWidgetsListMenuTooltip
                );
                if (SubMenu)
                {
                    using namespace ViewportWidgetsListSettings;
                    using namespace std;
                    auto AddSubMenus = [](UToolMenu* Menu, auto&& Self, shared_ptr<Node> HierarchyNode)
                    {
                        const UViewportWidgetsListSettings* PluginSettings = GetDefault<UViewportWidgetsListSettings>();
                        const UViewportWidgetsListUserSettings* PluginUserSettings = GetDefault<UViewportWidgetsListUserSettings>();
                        for (int32 i = 0; i < HierarchyNode->Entries.Num(); ++i)
                        {
                            const FViewportWidgetsListSettingsEntry& WidgetSettings = *HierarchyNode->Entries[i];
                            const TSoftObjectPtr<UEditorUtilityWidgetBlueprint>& Widget = WidgetSettings.Widget;
                            FName EntryName = FName(FString(TEXT("FViewportWidgetsListSettingsEntry_")) + FString::FromInt(i));
                            FText Label = WidgetSettings.Label.IsEmpty() ? WidgetSettings.DefaultLabel : WidgetSettings.Label;
                            FText TooltipText = WidgetSettings.TooltipText.IsEmpty() ? WidgetSettings.DefaultTooltipText : WidgetSettings.TooltipText;
                            if (!WidgetSettings.SectionName.IsNone() && !Menu->FindSection(WidgetSettings.SectionName))
                            {
                                Menu->AddSection(WidgetSettings.SectionName, FText::FromString(WidgetSettings.SectionName.ToString()));
                            }
                            Menu->AddMenuEntry(WidgetSettings.SectionName,
                                FToolMenuEntry::InitMenuEntry(EntryName, Label, TooltipText, FSlateIcon(),
                                    FUIAction(FExecuteAction::CreateLambda([Widget]()
                            {
                                UEditorUtilityWidgetBlueprint* Blueprint = Cast<UEditorUtilityWidgetBlueprint>(Widget.LoadSynchronous());
                                UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
                                EditorUtilitySubsystem->SpawnAndRegisterTab(Blueprint);
                            }))
                                )
                            );
                        }
                        for (const auto& [Key, Child] : HierarchyNode->Children)
                        {
                            FString Name = UTF8_TO_TCHAR(Key.c_str());
                            FToolMenuSection& SubMenuSection = Menu->FindOrAddSection(*Name);
                            SubMenuSection.AddSubMenu(*Name, FText::FromString(Name), FText::FromString(Name),
                                FNewToolMenuDelegate::CreateLambda([](UToolMenu* Menu, auto&& Self, shared_ptr<Node> HierarchyNode)
                            {
                                Self(Menu, Self, HierarchyNode);
                            }, Self, Child),
                                !PluginUserSettings->bOpenCustomSubmenuOnMouseHover,
                                FSlateIcon());
                        }
                    };
                    TArray<FName> SectionNames;
                    Algo::Transform(SubMenu->Sections, SectionNames, [](const FToolMenuSection& Section)
                    {
                        return Section.Name;
                    });
                    for (const FName& SectionName : SectionNames)
                    {
                        SubMenu->RemoveSection(SectionName);
                    }
                    AddSubMenus(SubMenu, AddSubMenus, PluginSettings->HierarchyRoot);
                }
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FViewportWidgetsListModule, ViewportWidgetsList);
DEFINE_LOG_CATEGORY(LogViewportWidgetsList);
