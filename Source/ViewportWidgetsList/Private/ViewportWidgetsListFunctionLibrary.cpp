// Copyright kiruru002. All Rights Reserved.

#include "ViewportWidgetsListFunctionLibrary.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsList.h"
#include "ViewportWidgetsList/Public/ViewportWidgetsListSettings.h"
#include "Components/Widget.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"

bool UViewportWidgetsListFunctionLibrary::WidgetIsChildOf(UWidget* Widget, UWidget* ParentWidget)
{
	return Widget && ParentWidget && Widget->IsChildOf(ParentWidget);
}

bool UViewportWidgetsListFunctionLibrary::WidgetIsIncludedIn(UWidget* Widget, UUserWidget* UserWidget)
{
	if (Widget && UserWidget)
	{
		UWidgetTree* WidgetTree = Cast<UWidgetTree>(Widget->GetOuter());
		if (WidgetTree == nullptr)
		{
			return false;
		}
		if (IsValid(WidgetTree->GetOuter()))
		{
			return WidgetTree->GetOuter() == UserWidget;
		}
	}
	return false;
}

void UViewportWidgetsListFunctionLibrary::GetWidgetHierarchyList(UWidget* StartingWidget, UWidget* StopWidget, TSubclassOf<UWidget> FindType, TArray<UWidget*>& List)
{
	if (!StartingWidget)
	{
		return;
	}
	if (StartingWidget->GetClass()->IsChildOf(FindType))
	{
		List.AddUnique(StartingWidget);
	}
	UWidget* CurrentWidget = StartingWidget;
	UWidget* Parent = FindParentWidgetOfType(CurrentWidget, FindType);
	while (Parent != nullptr && Parent != StopWidget)
	{
		List.AddUnique(Parent);
		CurrentWidget = Parent;
		Parent = FindParentWidgetOfType(CurrentWidget, FindType);
	}
}

UWidget* UViewportWidgetsListFunctionLibrary::FindParentWidgetOfType(UWidget* StartingWidget, TSubclassOf<UWidget> Type)
{
	while (IsValid(StartingWidget) && StartingWidget->GetFName().IsValid())
	{
		UWidget* LocalRoot = StartingWidget;
		UWidget* LocalParent = LocalRoot->GetParent();
		while (IsValid(LocalParent))
		{
			if (LocalParent->GetClass()->IsChildOf(Type))
			{
				return LocalParent;
			}
			LocalRoot = LocalParent;
			LocalParent = LocalParent->GetParent();
		}

		if (IsValid(LocalRoot->GetOuter()))
		{
			UWidgetTree* WidgetTree = Cast<UWidgetTree>(LocalRoot->GetOuter());
			if (WidgetTree == nullptr)
			{
				break;
			}

			if (IsValid(WidgetTree->GetOuter()))
			{
				StartingWidget = Cast<UUserWidget>(WidgetTree->GetOuter());
				if (StartingWidget && StartingWidget->GetClass()->IsChildOf(Type))
				{
					return StartingWidget;
				}
			}
		}
		StartingWidget = nullptr;
	}

	return nullptr;
}

bool UViewportWidgetsListFunctionLibrary::IsViewportWidgetsListPluginEnabled()
{
	const UViewportWidgetsListUserSettings* PluginUserSettings = GetDefault<UViewportWidgetsListUserSettings>();
	return PluginUserSettings->bEnableViewportWidgetsListPlugin;
}
