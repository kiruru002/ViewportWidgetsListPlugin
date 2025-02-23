// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList/Public/ViewportWidgetsListEditorSubsystem.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListFunctionLibrary.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Components/Button.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "Debugging/SlateDebugging.h"
#include "Types/ReflectionMetadata.h"
#include "Editor.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

void UViewportWidgetsListEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    if (UViewportWidgetsListFunctionLibrary::IsViewportWidgetsListPluginEnabled())
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
                Subsystem->CurrentPIEGameInstance = nullptr;
            }
        });
        OnStartGameInstanceHandle = FWorldDelegates::OnStartGameInstance.AddLambda([](UGameInstance* GameInstance)
        {
            if (GEditor)
            {
                UViewportWidgetsListEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UViewportWidgetsListEditorSubsystem>();
                Subsystem->CurrentPIEGameInstance = GameInstance;
                Subsystem->OnStartGameInstance.Broadcast(GameInstance);
            }
        });
        if (GEngine)
        {
            UGameViewportSubsystem* GameViewportSubsystem = GEngine->GetEngineSubsystem<UGameViewportSubsystem>();
            if (GameViewportSubsystem)
            {
                OnWidgetAddedHandle = GameViewportSubsystem->OnWidgetAdded.AddUObject(this, &UViewportWidgetsListEditorSubsystem::AddViewport);
                OnWidgetRemovedHandle = GameViewportSubsystem->OnWidgetRemoved.AddUObject(this, &UViewportWidgetsListEditorSubsystem::RemoveViewport);
            }
        }
#if WITH_SLATE_DEBUGGING
        InputEventHandle = FSlateDebugging::InputEvent.AddUObject(this, &UViewportWidgetsListEditorSubsystem::DebuggingInputEvent);
#endif
    }
    Super::Initialize(Collection);
}

void UViewportWidgetsListEditorSubsystem::Deinitialize()
{
    if (UViewportWidgetsListFunctionLibrary::IsViewportWidgetsListPluginEnabled() && OnPreBeginPIEHandle.IsValid())
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
        if (GEngine)
        {
            UGameViewportSubsystem* GameViewportSubsystem = GEngine->GetEngineSubsystem<UGameViewportSubsystem>();
            if (GameViewportSubsystem)
            {
                GameViewportSubsystem->OnWidgetAdded.Remove(OnWidgetAddedHandle);
                OnWidgetAddedHandle.Reset();
                GameViewportSubsystem->OnWidgetRemoved.Remove(OnWidgetRemovedHandle);
                OnWidgetRemovedHandle.Reset();
            }
        }
#if WITH_SLATE_DEBUGGING
        FSlateDebugging::InputEvent.Remove(InputEventHandle);
        InputEventHandle.Reset();
#endif
    }
    Super::Deinitialize();
}

void UViewportWidgetsListEditorSubsystem::AddViewport(UWidget* Widget, ULocalPlayer* LocalPlayer)
{
    ViewportWidgets.Add(Widget);
    OnWidgetAdded.Broadcast(LocalPlayer);
}

void UViewportWidgetsListEditorSubsystem::RemoveViewport(UWidget* Widget)
{
    ViewportWidgets.Remove(Widget);
    OnWidgetRemoved.Broadcast();
}

void UViewportWidgetsListEditorSubsystem::DebuggingInputEvent(const FSlateDebuggingInputEventArgs& EventArgs)
{
    if (CurrentPIEGameInstance)
    {
        if (EventArgs.InputEventType == ESlateDebuggingInputEvent::MouseButtonUp ||
            EventArgs.InputEventType == ESlateDebuggingInputEvent::MouseButtonDown ||
            EventArgs.InputEventType == ESlateDebuggingInputEvent::MouseButtonDoubleClick)
        {
            TSharedPtr<FReflectionMetaData> MetaData = FReflectionMetaData::GetWidgetOrParentMetaData(EventArgs.HandlerWidget.Get());
            if (MetaData.IsValid())
            {
                UWidget* Widget = Cast<UWidget>(MetaData->SourceObject.Get());
                if (IsValid(Widget))
                {
                    OnDebuggingInputEvent.Broadcast(EventArgs.InputEventType, EventArgs.Reply.IsEventHandled(), Widget, EventArgs.AdditionalContent);
                }
            }
        }
        else if (EventArgs.InputEventType == ESlateDebuggingInputEvent::MouseEnter)
        {
            TSharedPtr<FReflectionMetaData> MetaData = FReflectionMetaData::GetWidgetOrParentMetaData(EventArgs.HandlerWidget.Get());
            if (MetaData.IsValid())
            {
                UWidget* Widget = Cast<UWidget>(MetaData->SourceObject.Get());
                if (IsValid(Widget))
                {
                    // TODO: The order in which MouseEnter events are called is wrong, so we need to take action.
                    OnDebuggingInputEvent.Broadcast(EventArgs.InputEventType, EventArgs.Reply.IsEventHandled(), Widget, EventArgs.AdditionalContent);
                }
            }
        }
        else if (EventArgs.InputEventType == ESlateDebuggingInputEvent::MouseLeave)
        {
            TSharedPtr<FReflectionMetaData> MetaData = FReflectionMetaData::GetWidgetOrParentMetaData(EventArgs.HandlerWidget.Get());
            if (MetaData.IsValid())
            {
                UWidget* Widget = Cast<UWidget>(MetaData->SourceObject.Get());
                if (IsValid(Widget) && IsValid(Cast<UWidget>(Widget->GetParent())))
                {   
                    OnDebuggingInputEvent.Broadcast(EventArgs.InputEventType, EventArgs.Reply.IsEventHandled(), Cast<UWidget>(Widget->GetParent()), EventArgs.AdditionalContent);
                }
            }
        }
    }
}
