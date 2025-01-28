// Copyright kiruru002. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "Components/Widget.h"
#include "Debugging/SlateDebugging.h"
#include "ViewportWidgetsListEditorSubsystem.generated.h"

/**
 * Active viewport widgets list
 * https://zenn.dev/daria_nicht/articles/ue5-add-viewport-names
 */
UCLASS()
class VIEWPORTWIDGETSLIST_API UViewportWidgetsListEditorSubsystem : public UEditorSubsystem
{
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPIEEvent, bool, bIsSimulating);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameInstanceEvent, UGameInstance*, GameInstance);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetNumDecreased);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetNumIncreased, ULocalPlayer*, LocalPlayer);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDebuggingInputEvent, ESlateDebuggingInputEvent, InputEventType, bool, Handled, UWidget*, HandlerWidget, const FString&, AdditionalContent);

public:
    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnPIEEvent OnPreBeginPIE;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnPIEEvent OnBeginPIE;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnPIEEvent OnPrePIEEnded;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FOnPIEEvent OnEndPIE;

    UPROPERTY(BlueprintReadWrite, BlueprintAssignable, EditAnywhere, Category = "Default")
    FGameInstanceEvent OnStartGameInstance;

    FDelegateHandle OnPreBeginPIEHandle;
    FDelegateHandle OnBeginPIEHandle;
    FDelegateHandle OnPrePIEEndedHandle;
    FDelegateHandle OnEndPIEHandle;
    FDelegateHandle OnStartGameInstanceHandle;
    FDelegateHandle OnWidgetAddedHandle;
    FDelegateHandle OnWidgetRemovedHandle;
    FDelegateHandle InputEventHandle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UViewportWidgetsListEditorSubsystem")
    TArray<TObjectPtr<UWidget>> ViewportWidgets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UViewportWidgetsListEditorSubsystem")
    FOnWidgetNumIncreased OnWidgetAdded;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UViewportWidgetsListEditorSubsystem")
    FOnWidgetNumDecreased OnWidgetRemoved;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UViewportWidgetsListEditorSubsystem")
    FOnDebuggingInputEvent OnDebuggingInputEvent;

    UPROPERTY(BlueprintReadOnly, Category = "UViewportWidgetsListEditorSubsystem")
    UGameInstance* CurrentPIEGameInstance;

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    void AddViewport(UWidget* Widget, ULocalPlayer* LocalPlayer);
    void RemoveViewport(UWidget* Widget);
    void DebuggingInputEvent(const FSlateDebuggingInputEventArgs& EventArgs);

};
