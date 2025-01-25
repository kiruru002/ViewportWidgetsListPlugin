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

public:

    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetNumDecreased);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetNumIncreased, ULocalPlayer*, LocalPlayer);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnDebuggingInputEvent, ESlateDebuggingInputEvent, InputEventType, bool, Handled, UWidget*, HandlerWidget, const FString&, AdditionalContent);

public:
    void Initialize(FSubsystemCollectionBase& Collection) override;
    void AddViewport(UWidget* Widget, ULocalPlayer* LocalPlayer);
    void RemoveViewport(UWidget* Widget);
    void DebuggingInputEvent(const FSlateDebuggingInputEventArgs& EventArgs);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UViewportWidgets")
    TArray<TObjectPtr<UWidget>> ViewportWidgets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UViewportWidgets")
    FOnWidgetNumIncreased OnWidgetAdded;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UViewportWidgets")
    FOnWidgetNumDecreased OnWidgetRemoved;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = "UViewportWidgets")
    FOnDebuggingInputEvent OnDebuggingInputEvent;

    UWidget* LastEnteredWidget;
};
