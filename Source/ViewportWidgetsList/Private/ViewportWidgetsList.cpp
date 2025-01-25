// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FViewportWidgetsListModule"

void FViewportWidgetsListModule::StartupModule()
{
}

void FViewportWidgetsListModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FViewportWidgetsListModule, ViewportWidgetsList);
DEFINE_LOG_CATEGORY(LogViewportWidgetsList);
