#include "UI/TargetWidgetDisplayer.h"
#include "Blueprint/UserWidget.h"
#include "Debug/UELOGCategories.h"

UTargetWidgetDisplayer::UTargetWidgetDisplayer()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTargetWidgetDisplayer::BeginPlay()
{
	Super::BeginPlay();
}

void UTargetWidgetDisplayer::CreateWidget(FName StorageName, FName WidgetName)
{
	FWidgetStorage* Storage = WidgetLists.Find(StorageName);
	if (!Storage)
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::CreateWidget -> Storage %s not found"),
			*StorageName.ToString());
		return;
	}

	// Check if instance already exists
	if (Storage->WidgetInstances.Contains(WidgetName))
	{
		UE_LOG(UI_Log, Warning, TEXT(
			"UTargetWidgetDisplayer::CreateWidget -> Widget %s already created in %s"),
			*WidgetName.ToString(), *StorageName.ToString());
		return;
	}

	// Get the widget class
	TSubclassOf<UUserWidget>* ClassPtr = Storage->WidgetClasses.Find(WidgetName);
	if (!ClassPtr || !(*ClassPtr))
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::CreateWidget -> Widget class %s not found or invalid in %s"),
			*WidgetName.ToString(), *StorageName.ToString());
		return;
	}

	// Create widget instance
	UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetOwner(), *ClassPtr);
	if (!NewWidget)
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::CreateWidget -> Failed to create %s"),
			*WidgetName.ToString());
		return;
	}

	// Store the instance
	Storage->WidgetInstances.Add(WidgetName, NewWidget);

	UE_LOG(UI_Log, Log, TEXT(
		"UTargetWidgetDisplayer::CreateWidget -> %s created and stored in %s"),
		*WidgetName.ToString(), *StorageName.ToString());
}

void UTargetWidgetDisplayer::ShowWidget(FName StorageName, FName WidgetName, int32 Order)
{
	FWidgetStorage* Storage = WidgetLists.Find(StorageName);//find storage
	if (!Storage)
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::ShowWidget -> Storage %s not found"),
			*StorageName.ToString());
		return;
	}

	UUserWidget** FoundWidget = Storage->WidgetInstances.Find(WidgetName);// check if the widget is already created and if it is valid or not
	if (!FoundWidget || !(*FoundWidget))
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::ShowWidget -> Widget %s not found in %s"),
			*WidgetName.ToString(), *StorageName.ToString());
		return;
	}
	
	// All checked --> renter to the viewport
	
	UUserWidget* Widget = *FoundWidget;
	if (!Widget->IsInViewport())// for last check. check if the widget is on viewport or not
	{
		Widget->AddToViewport(Order);
		UE_LOG(UI_Log, Log, TEXT(
			"UTargetWidgetDisplayer::ShowWidget -> [%s] shown with order %d"),
			*WidgetName.ToString(), Order);
	}
}

void UTargetWidgetDisplayer::HideWidget(FName StorageName, FName WidgetName)
{
	FWidgetStorage* Storage = WidgetLists.Find(StorageName);//find where to find
	if (!Storage)
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::HideWidget -> Storage %s not found"),
			*StorageName.ToString());
		return;
	}

	UUserWidget** FoundWidget = Storage->WidgetInstances.Find(WidgetName);
	if (!FoundWidget || !(*FoundWidget))
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::HideWidget -> Widget %s not found in %s"),
			*WidgetName.ToString(), *StorageName.ToString());
		return;
	}

	UUserWidget* Widget = *FoundWidget;
	if (Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
		UE_LOG(UI_Log, Log, TEXT(
			"UTargetWidgetDisplayer::HideWidget -> [%s] hidden"),
			*WidgetName.ToString());
	}
}

void UTargetWidgetDisplayer::RemoveWidget(FName StorageName, FName WidgetName)
{
	FWidgetStorage* Storage = WidgetLists.Find(StorageName);// again, find storage
	if (!Storage)
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::RemoveWidget -> Storage %s not found"),
			*StorageName.ToString());
		return;
	}

	UUserWidget* Widget = nullptr;// find widget to be removed
	if (UUserWidget** FoundWidget = Storage->WidgetInstances.Find(WidgetName))//fucking **
	{
		Widget = *FoundWidget;
	}

	if (!Widget)
	{
		UE_LOG(UI_Log, Warning, TEXT(
			"UTargetWidgetDisplayer::RemoveWidget -> Widget %s not found in %s"),
			*WidgetName.ToString(), *StorageName.ToString());
		return;
	}

	if (Widget->IsInViewport())// already has an function to check if the widget is on screen or not fuck
	{
		Widget->RemoveFromParent();
	}

	Storage->WidgetInstances.Remove(WidgetName);

	UE_LOG(UI_Log, Log, TEXT(
		"UTargetWidgetDisplayer::RemoveWidget -> %s removed from %s"),
		*WidgetName.ToString(), *StorageName.ToString());
}

void UTargetWidgetDisplayer::SetAnchorForWidgets(FName StorageName, USceneComponent* NewAnchor)
{
	FWidgetStorage* Storage = WidgetLists.Find(StorageName);
	if (!Storage)
	{
		UE_LOG(UI_Log, Error, TEXT(
			"UTargetWidgetDisplayer::SetAnchorForWidgets -> Storage %s not found"),
			*StorageName.ToString());
		return;
	}

	Storage->AnchorPoint = NewAnchor;
	UE_LOG(UI_Log, Log, TEXT(
		"UTargetWidgetDisplayer::SetAnchorForWidgets -> Anchor set for %s"),
		*StorageName.ToString());
}
