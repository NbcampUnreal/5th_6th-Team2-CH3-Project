// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManager.h"
#include "Controller/PlayerController/MyPlayerController.h"
#include "Debug/UELOGCategories.h"

// Sets default values for this component's properties
UUIManagerComponent::UUIManagerComponent():
	HUDWidgets(FWidgetStorage(EUIMode::Gameplay)),
	Menuidgets(FWidgetStorage(EUIMode::Menu)),

	CurrentUIMode(EUIMode::None)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UUIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UUIManagerComponent::AddWidget(EUIMode Mode, FName WidgetName, TSubclassOf<UUserWidget> WidgetClass)
{
	//=== Valid Check ===//
	FWidgetStorage* Storage=nullptr;
	if (!GetwidgetStorageByMode(Mode,Storage)) return;

	if (!IsWidgetValid(WidgetName,WidgetClass,*Storage)) return;

	if (!OwnerController)
	{
		UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::AddHUDWidget->InvalidController"));
		return;
	}

	FWidgetData NewW_Data;
	NewW_Data.WidgetClass = WidgetClass;// instance and bvisible is all default nullptr, and false

	Storage->StoredWidgets.Add(WidgetName,NewW_Data);
}

void UUIManagerComponent::RemoveWidget(EUIMode Mode, FName WidgetName)
{
	//===== Valid Check
	FWidgetStorage* Storage=nullptr;
	if (!GetwidgetStorageByMode(Mode,Storage)) return;

	FWidgetData* Data= Storage->StoredWidgets.Find(WidgetName);
	if (!Data)
	{
		UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::RemoveWidget->Could not find %s"), *WidgetName.ToString());
		return;
	}

	if (Data->WidgetInstance)
	{
		Data->WidgetInstance->RemoveFromParent();
		Data->WidgetInstance=nullptr;
	}
	else
	{
		UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::RemoveWidget-> %s is not valid"), *WidgetName.ToString());
		// but remove it from the storage anyway
	}

	Storage->StoredWidgets.Remove(WidgetName);
	UE_LOG(UI_Log,Log, TEXT(" UUIManagerComponent::RemoveWidget-> %s removed"), *WidgetName.ToString());
}

void UUIManagerComponent::ShowWidget(EUIMode Mode, FName WidgetName, int32 Order)
{
	FWidgetStorage* Storage=nullptr;
	if (!GetwidgetStorageByMode(Mode,Storage)) return;

	FWidgetData* Data= Storage->StoredWidgets.Find(WidgetName);
	if (!Data)
	{
		UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::ShowWidget->Could not find %s"), *WidgetName.ToString());
		return;
	}

	if (!Data->WidgetInstance && Data->WidgetClass)
	{
		Data->WidgetInstance=CreateWidget(GetWorld(), Data->WidgetClass);
		if (!Data->WidgetInstance)
		{
			UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::ShowWidget->Failed to Create %s"), *WidgetName.ToString())
			return;
		}
	}

	if (Data->WidgetInstance && !Data->bIsVisible)
	{
		Data->WidgetInstance->AddToViewport(Order);
		Data->bIsVisible = true;
	}
}

void UUIManagerComponent::HideWidget(EUIMode Mode, FName WidgetName)
{
	FWidgetStorage* Storage=nullptr;
	if (!GetwidgetStorageByMode(Mode,Storage)) return;

	FWidgetData* Data= Storage->StoredWidgets.Find(WidgetName);
	if (!Data->bIsVisible)
	{
		UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::HideWidget-> %s is already visible"), *WidgetName.ToString())
		return;
	}
	
	Data->WidgetInstance->RemoveFromParent();
	Data->bIsVisible = false;
}

void UUIManagerComponent::ClearWidgetsByMode(EUIMode Mode)
{
	FWidgetStorage* Temp=nullptr;
	if (!GetwidgetStorageByMode(Mode,Temp)) return;
	
	
	for (TPair<FName, FWidgetData>&Pair : Temp->StoredWidgets)
	{
		if (Pair.Value.WidgetInstance)
		{
			Pair.Value.WidgetInstance->RemoveFromParent();
			Pair.Value.WidgetInstance=nullptr;
		}
	}
	Temp->StoredWidgets.Empty();
}

void UUIManagerComponent::ClearAllWidgets()
{
	ClearWidgetsByMode(EUIMode::Gameplay);
	ClearWidgetsByMode(EUIMode::Menu);
}

bool UUIManagerComponent::GetwidgetStorageByMode(EUIMode Mode, FWidgetStorage*& WidgetStorage)
{
	switch (Mode)
	{
	case EUIMode::Gameplay:
		WidgetStorage=&HUDWidgets;
		return true;
	case EUIMode::Menu:
		WidgetStorage=&Menuidgets;
		return true;
	case EUIMode::None:
	default:
		UE_LOG(UI_Log, Error, TEXT(" UUIManagerComponent::GetwidgetStorageByMode-> Invalid Mode"));
		WidgetStorage=nullptr;
		return false;
	}
}



bool UUIManagerComponent::IsWidgetValid(FName WidgetName, TSubclassOf<UUserWidget> WidgetClass, FWidgetStorage& WidgetStorage)
{
	if (!WidgetClass)
	{
		UE_LOG(UI_Log,Error,TEXT(" UUIManagerComponent::IsWidgetValid->Invalid WidgetClass"));
		return false;
	}
	if (WidgetName==NAME_None)
	{
		UE_LOG(UI_Log,Error,TEXT(" UUIManagerComponent::IsWidgetValid->Invalid Name"));
		return false;
	}
	if (WidgetStorage.StoredWidgets.Contains(WidgetName))
	{
		UE_LOG(UI_Log,Error,TEXT(" UUIManagerComponent::IsWidgetValid->Invalid Name"));
		return false;
	}

	return true;
}

void UUIManagerComponent::ActivateUIManager(AMyPlayerController* MyController)
{
	if (!MyController)
	{
		UE_LOG(UI_Log,Error,TEXT("UUIManagerComponent::ActivateUIManager->Invalid Controller Detected"));
		return;
	}

	OwnerController = MyController;
}

void UUIManagerComponent::DeactivateUIManager()
{
	ClearAllWidgets();
	OwnerController = nullptr;
}

void UUIManagerComponent::ApplyInputModeByCurrentUIMode()
{
	if (!OwnerController)
	{
		UE_LOG(UI_Log,Error,TEXT(" UUIManagerComponent::ApplyInputModeByCurrentUIMode->Invalid OwnerController"));
		return;
	}

	//TODO--> Set inputmode for menu and hud. also differenciate them by input type gamepad or pc
}




