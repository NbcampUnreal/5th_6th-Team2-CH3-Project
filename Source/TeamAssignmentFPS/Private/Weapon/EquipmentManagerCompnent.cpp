// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EquipmentManagerCompnent.h"

#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "Interface/InputReactionInterface.h"

#include "Debug/UELOGCategories.h"//debug log


// Sets default values for this component's properties
UEquipmentManagerCompnent::UEquipmentManagerCompnent():
	CurrentWeapon(nullptr),
	CurrentItem(nullptr),
	CurrentEquipment(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UEquipmentManagerCompnent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UEquipmentManagerCompnent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UEquipmentManagerCompnent::SwtichWeapon(const FInputActionValue& Value)
{
}

void UEquipmentManagerCompnent::SelectItem_PC(const FInputActionValue& Value)
{
}

void UEquipmentManagerCompnent::SelectItem_GP(const FInputActionValue& Value)
{
}

void UEquipmentManagerCompnent::TriggerInput_Start(const FInputActionValue& Value)
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("EquipmentManagerCompnent::TriggerInput_Start-> CurrentEquipment is invalid"));
		return;
	}
	bDidHoldStarted=true;
	CurrentHoldingTime=0.0f;

	const bool bDidItWork = FInputTypeHelper::TryCallingInterface(
		CurrentEquipment,
		[](UObject* Equipment)
	{
		IInputReactionInterface::Execute_OnInputPressed(Equipment);
		IInputReactionInterface::Execute_OnInputHoldStart(Equipment);
	});

	if (!bDidItWork)// when calling failed
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("EquipmentManagerCompnent::TriggerInput_Start-> CurrentEquipment does not have required interface"));
		// TODO:
		// siganl ui that equipment trigger is invalid (ex. red highlight for current equipment widget)
		// player character playing anim for invalid command?
	}
		
}

void UEquipmentManagerCompnent::TriggerInput_Trigger(const FInputActionValue& Value)
{
	if (!CurrentEquipment)
	{
		//UE_LOG(Equipment_Manager_Log, Error,TEXT("UEquipmentManagerCompnent::TriggerInput_Trigger-> CurrentEquipment is invalid"));
		// no equipment to update
		return;
	}
	if (!bDidHoldStarted)
	{
		// not even started yet
		return;
	}
	
	CurrentHoldingTime += GetWorld()->GetDeltaSeconds();// update holding time

	// Optionally trigger HoldUpdate events
	const bool bDidItWork = FInputTypeHelper::TryCallingInterface(
		CurrentEquipment,
		[this](UObject* Equipment)
	{
		IInputReactionInterface::Execute_OnInputHoldUpdate(Equipment, CurrentHoldingTime);
	});

	if (!bDidItWork)
	{
		//error updating not working
	}
}

void UEquipmentManagerCompnent::TriggerInput_Complete(const FInputActionValue& Value)
{
	if (!CurrentEquipment) return;

	FInputTypeHelper::HandleTapOrHoldRelease(CurrentEquipment, CurrentHoldingTime, TapThreshold);

	// Reset the variables
	CurrentHoldingTime = 0.f;
	bDidHoldStarted = false;
}

void UEquipmentManagerCompnent::TriggerInput_Ongoing(const FInputActionValue& Value)
{
	// not decided yet. maybe update the ui?
}

void UEquipmentManagerCompnent::TriggerInput_Canceled(const FInputActionValue& Value)// not so sure where to use this
{
	if (!CurrentEquipment)
	{
		//error
		return;
	}

	// should this be treated as a release or just cancle and dont trigger the function? not so sure fuck
	IInputReactionInterface::Execute_OnInputRelease(CurrentEquipment);

	CurrentHoldingTime = 0.f;
	bDidHoldStarted = false;
}
