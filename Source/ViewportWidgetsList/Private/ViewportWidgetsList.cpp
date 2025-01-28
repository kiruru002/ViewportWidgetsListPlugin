// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "Modules/ModuleManager.h"
#include "Templates/SubclassOf.h"
#include "Components/Widget.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "EditorUtilitySubsystem.h"
#include "Editor.h"
#include "ISettingsModule.h"
#include "ToolMenus.h"

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
    const UViewportWidgetsListUserSettings* PluginUserSettings = GetDefault<UViewportWidgetsListUserSettings>();
    const UViewportWidgetsListSettings* PluginSettings = GetDefault<UViewportWidgetsListSettings>();
    if (PluginUserSettings->bEnableViewportWidgetsListPlugin && PluginSettings)
    {
        // メニューシステムの初期化
        if (!UToolMenus::IsToolMenuUIEnabled())
        {
            return;
        }
        UToolMenus* ToolMenus = UToolMenus::Get();
        // メインメニューの Tools セクションに項目を追加
        {
            UToolMenu* Menu = ToolMenus->ExtendMenu("Mainframe.MainMenu");
            if (Menu)
            {
                UToolMenu* SubMenu = Menu->AddSubMenu(Menu,
                    NAME_None,
                    TEXT("FViewportWidgetsListModule_Menu"),
                    LOCTEXT("FViewportWidgetsListModule_Menu_Label", "EUW"),
                    LOCTEXT("FViewportWidgetsListModule_Menu_Description", "EUW Menu.")
                );
                if (SubMenu)
                {
                    for (const TSoftObjectPtr<UEditorUtilityWidgetBlueprint>& Widget : PluginSettings->ViewportWidgetsListMenuProvidedWidgets)
                    {
                        FString Path = Widget.ToSoftObjectPath().GetAssetPathName().ToString();
                        FName EntryName = FName(FString(TEXT("OpenEUW_")) + Path);
                        FText Label = FText::Format(LOCTEXT("OpenEUW_Label", "Open {0}"), FText::FromString(FPaths::GetBaseFilename(Path)));
                        FText Description = FText::Format(LOCTEXT("OpenEUW_Description", "Open {0}."), FText::FromString(FPaths::GetBaseFilename(Path)));
                        SubMenu->AddMenuEntry(NAME_None,
                            FToolMenuEntry::InitMenuEntry(EntryName, Label, Description, FSlateIcon(),
                                FUIAction(FExecuteAction::CreateLambda([Widget]()
                        {
                            UEditorUtilityWidgetBlueprint* Blueprint = Cast<UEditorUtilityWidgetBlueprint>(Widget.LoadSynchronous());
                            UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
                            EditorUtilitySubsystem->SpawnAndRegisterTab(Blueprint);
                        }))
                            )
                        );
                    }
                }
            }
        }
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FViewportWidgetsListModule, ViewportWidgetsList);
DEFINE_LOG_CATEGORY(LogViewportWidgetsList);
