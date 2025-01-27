// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "Modules/ModuleManager.h"
#include "ISettingsModule.h"

#define LOCTEXT_NAMESPACE "FViewportWidgetsListModule"

void FViewportWidgetsListModule::StartupModule()
{
#if WITH_EDITOR
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    if (SettingsModule)
    {
        SettingsModule->RegisterSettings("Editor", "Plugins", "ViewportWidgetsListSettings",
            LOCTEXT("ViewportWidgetsListSettings_Name", "UViewportWidgetsListSettings"),
            LOCTEXT("ViewportWidgetsListSettings_Description", "ViewportWidgetsListPlugin's Settings"),
            GetMutableDefault<UViewportWidgetsListSettings>()
        );
    }
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FViewportWidgetsListModule, ViewportWidgetsList);
DEFINE_LOG_CATEGORY(LogViewportWidgetsList);
