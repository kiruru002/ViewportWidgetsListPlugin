// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Logging/LogMacros.h"

class FViewportWidgetsListModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    /** ~IModuleInterface implementation */
    void RegisterMenus();
};

DECLARE_LOG_CATEGORY_EXTERN(LogViewportWidgetsList, Log, All);
