// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManagerComponent.h"
#include "Controller/PlayerController/MyPlayerController.h"
#include "Debug/UELOGCategories.h"
#include "Blueprint/UserWidget.h"
#include "Time/TimeControlHelper.h"// for pause and unpause, speed control
#include "UI/InputTypeReactInterface.h"

// Sets default values for this component's properties
UUIManagerComp::UUIManagerComp():
	//Controller
	OwnerController(nullptr),
	HUDWidgets(FWidgetDataStorage(EUIMode::Gameplay)),
	MenuWidgets(FWidgetDataStorage(EUIMode::Menu)),
	CurrentUIMode(EUIMode::None)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


// Called when the game starts
void UUIManagerComp::BeginPlay()
{
	Super::BeginPlay();

	
}

void UUIManagerComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UUIManagerComp::AddWidget(EUIMode Mode, FName WidgetName, TSubclassOf<UUserWidget> WidgetClass)
{
	//=== Valid Check ===//
	FWidgetDataStorage* Storage=nullptr;
	if (!GetWidgetStorageByMode(Mode,Storage)) return;

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

void UUIManagerComp::RemoveWidget(EUIMode Mode, FName WidgetName)
{
	//===== Valid Check
	FWidgetDataStorage* Storage=nullptr;
	if (!GetWidgetStorageByMode(Mode,Storage)) return;

	FWidgetData* Data= Storage->StoredWidgets.Find(WidgetName);
	if (!Data)
	{
		UE_LOG(UI_Log, Error,
			TEXT(" UUIManagerComponent::RemoveWidget->Could not find %s"), *WidgetName.ToString());
		return;
	}

	if (Data->WidgetInstance)
	{
		Data->WidgetInstance->RemoveFromParent();
		Data->WidgetInstance=nullptr;
	}
	else
	{
		UE_LOG(UI_Log, Error,
			TEXT(" UUIManagerComponent::RemoveWidget-> %s is not valid"), *WidgetName.ToString());
		// but remove it from the storage anyway
	}

	Storage->StoredWidgets.Remove(WidgetName);
	UE_LOG(UI_Log,Log,
		TEXT(" UUIManagerComponent::RemoveWidget-> %s removed"), *WidgetName.ToString());
}

void UUIManagerComp::ShowWidget(EUIMode Mode, FName WidgetName, int32 Order)
{
	FWidgetDataStorage* Storage=nullptr;
	if (!GetWidgetStorageByMode(Mode,Storage)) return;

	FWidgetData* Data= Storage->StoredWidgets.Find(WidgetName);
	if (!Data)
	{
		UE_LOG(UI_Log, Error,
			TEXT(" UUIManagerComponent::ShowWidget->Could not find %s"), *WidgetName.ToString());
		return;
	}

	if (!Data->WidgetInstance && Data->WidgetClass)
	{
		Data->WidgetInstance=CreateWidget(GetWorld(), Data->WidgetClass);
		if (!Data->WidgetInstance)
		{
			UE_LOG(UI_Log, Error,
				TEXT(" UUIManagerComponent::ShowWidget->Failed to Create %s"), *WidgetName.ToString())
			return;
		}
	}

	if (Data->WidgetInstance && !Data->bIsVisible)
	{
		Data->WidgetInstance->AddToViewport(Order);
		Data->bIsVisible = true;
	}
}

void UUIManagerComp::HideWidget(EUIMode Mode, FName WidgetName)
{
	FWidgetDataStorage* Storage=nullptr;
	if (!GetWidgetStorageByMode(Mode,Storage)) return;

	FWidgetData* Data= Storage->StoredWidgets.Find(WidgetName);
	if (!Data->bIsVisible)
	{
		UE_LOG(UI_Log, Error,
			TEXT(" UUIManagerComponent::HideWidget-> %s is already visible"), *WidgetName.ToString())
		return;
	}
	
	Data->WidgetInstance->RemoveFromParent();
	Data->bIsVisible = false;
}

void UUIManagerComp::ClearWidgetsByMode(EUIMode Mode)
{
	FWidgetDataStorage* Temp=nullptr;
	if (!GetWidgetStorageByMode(Mode,Temp)) return;
	
	
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

void UUIManagerComp::ClearAllWidgets()
{
	ClearWidgetsByMode(EUIMode::Gameplay);
	ClearWidgetsByMode(EUIMode::Menu);
}

bool UUIManagerComp::GetWidgetStorageByMode(EUIMode Mode, FWidgetDataStorage*& WidgetStorage)
{
	switch (Mode)
	{
	case EUIMode::Gameplay:
		WidgetStorage=&HUDWidgets;
		return true;
	case EUIMode::Menu:
		WidgetStorage=&MenuWidgets;
		return true;
	case EUIMode::None:
	default:
		UE_LOG(UI_Log, Error,
			TEXT(" UUIManagerComponent::GetwidgetStorageByMode-> Invalid Mode"));
		WidgetStorage=nullptr;
		return false;
	}
}



bool UUIManagerComp::IsWidgetValid(FName WidgetName, TSubclassOf<UUserWidget> WidgetClass, FWidgetDataStorage& WidgetStorage)
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

void UUIManagerComp::ActivateUIManager(AMyPlayerController* MyController)
{
	if (!MyController)
	{
		UE_LOG(UI_Log,Error,TEXT("UUIManagerComponent::ActivateUIManager->Invalid Controller Detected"));
		return;
	}

	OwnerController = MyController;
}

void UUIManagerComp::DeactivateUIManager()
{
	ClearAllWidgets();
	OwnerController = nullptr;
}

void UUIManagerComp::SwitchUIMode(EUIMode Mode)
{
	if (CurrentUIMode==Mode)
	{
		UE_LOG(UI_Log, Error, TEXT("UUIManagerComp::SwitchUIMode-> Already in %s mode"),
		*StaticEnum<EUIMode>()->GetDisplayNameTextByValue(static_cast<int64>(Mode)).ToString());
		return;
	}

	CurrentUIMode=Mode;
	//open and close menu

	switch (Mode)
	{
	case EUIMode::Gameplay:
		UTimeControlHelper::SetGlobalTimeDilation(this,1);// set to normal speed
		break;

	case EUIMode::Menu:

		UTimeControlHelper::SetGlobalTimeDilation(this,0);//pause
		break;

	default:// when it is none//--> should this be a setting when there are absolutely no widgets?
		break;

	}
	
	ApplyInputModeByCurrentUIMode();
}


void UUIManagerComp::ApplyInputModeByCurrentUIMode()
{
	if (!OwnerController)
	{
		UE_LOG(UI_Log,Error,TEXT(" UUIManagerComponent::ApplyInputModeByCurrentUIMode->Invalid OwnerController"));
		return;
	}

	
}




