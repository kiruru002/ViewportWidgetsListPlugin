// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList.h"
#include "Editor.h"
#include "Modules/ModuleManager.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListEditorSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

#define LOCTEXT_NAMESPACE "FViewportWidgetsListModule"



void FViewportWidgetsListModule::StartupModule()
{
    OnPreBeginPIEHandle = FEditorDelegates::PreBeginPIE.AddLambda([](const bool bIsSimulating)
    {
        if (GEditor)
        {
            UViewportWidgetsListEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UViewportWidgetsListEditorSubsystem>();
            Subsystem->OnPreBeginPIE.Broadcast(bIsSimulating);
        }
    });
    OnBeginPIEHandle = FEditorDelegates::BeginPIE.AddLambda([](const bool bIsSimulating)
    {
        if (GEditor)
        {
            UViewportWidgetsListEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UViewportWidgetsListEditorSubsystem>();
            Subsystem->OnBeginPIE.Broadcast(bIsSimulating);
        }
    });
    OnPrePIEEndedHandle = FEditorDelegates::PrePIEEnded.AddLambda([](const bool bIsSimulating)
    {
        if (GEditor)
        {
            UViewportWidgetsListEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UViewportWidgetsListEditorSubsystem>();
            Subsystem->OnPrePIEEnded.Broadcast(bIsSimulating);
        }
    });
    OnEndPIEHandle = FEditorDelegates::EndPIE.AddLambda([](const bool bIsSimulating)
    {
        if (GEditor)
        {
            UViewportWidgetsListEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UViewportWidgetsListEditorSubsystem>();
            Subsystem->OnEndPIE.Broadcast(bIsSimulating);
        }
    });
    OnStartGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddLambda([](UGameInstance* GameInstance)
    {
        if (GEditor)
        {
            UViewportWidgetsListEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UViewportWidgetsListEditorSubsystem>();
            Subsystem->OnStartGameInstance.Broadcast(GameInstance);
        }
    });
}

void FViewportWidgetsListModule::ShutdownModule()
{
    FEditorDelegates::PreBeginPIE.Remove(OnPreBeginPIEHandle);
    OnPreBeginPIEHandle.Reset();
    FEditorDelegates::PreBeginPIE.Remove(OnBeginPIEHandle);
    OnBeginPIEHandle.Reset();
    FEditorDelegates::PrePIEEnded.Remove(OnPrePIEEndedHandle);
    OnPrePIEEndedHandle.Reset();
    FEditorDelegates::EndPIE.Remove(OnEndPIEHandle);
    OnEndPIEHandle.Reset();
    FWorldDelegates::OnStartGameInstance.Remove(OnStartGameInstanceHandle);
    OnStartGameInstanceHandle.Reset();
}

IMPLEMENT_MODULE(FViewportWidgetsListModule, ViewportWidgetsList);
DEFINE_LOG_CATEGORY(LogViewportWidgetsList);
