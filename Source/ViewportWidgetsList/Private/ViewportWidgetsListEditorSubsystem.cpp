// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsList/Public/ViewportWidgetsListEditorSubsystem.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListFunctionLibrary.h"
#include "Blueprint/GameViewportSubsystem.h"
#include "Components/Button.h"
#include "Debugging/SlateDebugging.h"
#include "Types/ReflectionMetadata.h"

void UViewportWidgetsListEditorSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    LastEnteredWidget = nullptr;
    auto Subsystem = GEngine->GetEngineSubsystem<UGameViewportSubsystem>();
    Subsystem->OnWidgetAdded.AddUObject(this, &UViewportWidgetsListEditorSubsystem::AddViewport);
    Subsystem->OnWidgetRemoved.AddUObject(this, &UViewportWidgetsListEditorSubsystem::RemoveViewport);
#if WITH_SLATE_DEBUGGING
    FSlateDebugging::InputEvent.AddUObject(this, &UViewportWidgetsListEditorSubsystem::DebuggingInputEvent);
#endif
    Super::Initialize(Collection);
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
                if (UViewportWidgetsListFunctionLibrary::FindParentWidgetOfType(LastEnteredWidget, Widget->GetClass()) != Widget)
                {
                    LastEnteredWidget = Widget;
                    OnDebuggingInputEvent.Broadcast(EventArgs.InputEventType, EventArgs.Reply.IsEventHandled(), LastEnteredWidget, EventArgs.AdditionalContent);
                }
                else
                {
                    UE_LOG(LogViewportWidgetsList, Verbose, TEXT("%s: %s is parent of %s"), ANSI_TO_TCHAR(__FUNCTION__), *Widget->GetName(), *LastEnteredWidget->GetName());
                }
            }
        }
    }
    else if (EventArgs.InputEventType == ESlateDebuggingInputEvent::MouseLeave)
    {
        TSharedPtr<FReflectionMetaData> MetaData = FReflectionMetaData::GetWidgetOrParentMetaData(EventArgs.HandlerWidget.Get());
        if (MetaData.IsValid())
        {
            UWidget* Widget = Cast<UWidget>(MetaData->SourceObject.Get());
            if (IsValid(Widget) && Widget == LastEnteredWidget)
            {
                LastEnteredWidget = UViewportWidgetsListFunctionLibrary::FindParentWidgetOfType(LastEnteredWidget, UWidget::StaticClass());
                if (IsValid(LastEnteredWidget))
                {
                    OnDebuggingInputEvent.Broadcast(EventArgs.InputEventType, EventArgs.Reply.IsEventHandled(), LastEnteredWidget, EventArgs.AdditionalContent);
                }
            }
        }
    }
}
