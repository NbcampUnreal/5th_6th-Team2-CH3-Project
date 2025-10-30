// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EquipmentManagerCompnent.h"

#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "InputAction.h"
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

void UEquipmentManagerCompnent::UpdatePlacementComponent(USceneComponent* NewPlacement)
{
	if (!NewPlacement)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerCompnent::UpdatePlacementComponent-> Invalid Placedment"));
		return;
	}
	
	Placement=NewPlacement;
	SetCurrentEquipmentPlacement();
}


void UEquipmentManagerCompnent::SetCurrentEquipmentPlacement()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerCompnent::SetCurrentEquipmentPlacement-> no equipment to place"));
		return;
	}
	if (!Placement)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerCompnent::SetCurrentEquipmentPlacement-> no place to equip"));
		return;
	}

	CurrentEquipment->AttachToComponent(Placement, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CurrentEquipment->SetActorRelativeLocation(FVector::ZeroVector);//offset
	CurrentEquipment->SetActorRelativeRotation(FRotator::ZeroRotator);//offset
}

void UEquipmentManagerCompnent::SwtichWeapon_PC(const FInputActionValue& Value)
{
	/*float CurrentMouseWheelValue = Value.Get<float>();
	UE_LOG(Equipment_Manager_Log, Log, TEXT("UEquipmentManagerCompnent::SwtichWeapon_PC-> Value:%f"),CurrentMouseWheelValue);//temp
	
	// Detect first movement of the wheel (from 0 to non-zero)
	if (PreviousMouseWheelValue == 0.f && CurrentMouseWheelValue != 0.f)
	{
		if (CurrentMouseWheelValue > 0.f)
		{
			// Scroll up -> next weapon
			UE_LOG(Equipment_Manager_Log, Log, TEXT("UEquipmentManagerCompnent::SwtichWeapon_PC -> [+]"));
		}
		else
		{
			// Scroll down -> previous weapon
			UE_LOG(Equipment_Manager_Log, Log, TEXT("UEquipmentManagerCompnent::SwtichWeapon_PC -> [-]"));
		}
	}

	// Store the current value for next frame
	PreviousMouseWheelValue = CurrentMouseWheelValue;*/
	/*float CurrentValue = Value.Get<float>();

	if (CurrentValue != 0.f)
	{
		// Check if previous value had a different sign or was zero
		if ((PreviousMouseWheelValue <= 0.f && CurrentValue > 0.f) || 
			(PreviousMouseWheelValue >= 0.f && CurrentValue < 0.f))
		{
			if (CurrentValue > 0.f)
			{
				UE_LOG(Equipment_Manager_Log, Log, TEXT("SwitchWeapon_PC -> Next weapon"));
			}
			else
			{
				UE_LOG(Equipment_Manager_Log, Log, TEXT("SwitchWeapon_PC -> Previous weapon"));
			}

			// Reset previous value so next scroll gesture can trigger again
			PreviousMouseWheelValue = CurrentValue;
		}
	}
	else
	{
		// Wheel is idle, reset
		PreviousMouseWheelValue = 0.f;
	}*/
	
	float WheelDelta = Value.Get<float>();
	if (WheelDelta == 0.f) return;

	PreviousMouseWheelValue += WheelDelta;

	while (PreviousMouseWheelValue >= 1.f)
	{
		// Scroll up -> next weapon
		UE_LOG(Equipment_Manager_Log, Log, TEXT("Next Weapon"));
		PreviousMouseWheelValue -= 1.f;
	}

	while (PreviousMouseWheelValue <= -1.f)
	{
		// Scroll down -> previous weapon
		UE_LOG(Equipment_Manager_Log, Log, TEXT("Previous Weapon"));
		PreviousMouseWheelValue += 1.f;
	}
}

void UEquipmentManagerCompnent::SwtichWeapon_GP(const FInputActionValue& Value)
{
}

void UEquipmentManagerCompnent::SelectItem_PC(const FInputActionValue& Value)
{
}

void UEquipmentManagerCompnent::SelectItem_GP(const FInputActionValue& Value)
{
}

void UEquipmentManagerCompnent::TriggerInput_Reload(const FInputActionValue& Value)
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("EquipmentManagerCompnent::TriggerInput_Reload-> CurrentEquipment is invalid"));
		return;
	}

	const bool bDidItWork = FInputTypeHelper::TryCallingInterface(CurrentEquipment,
		[](UObject* Equipment)
	{
		// Call reload to the currentdquipment
		IWeaponInterface::Execute_OnReloadInputPressed(Equipment);
	});

	if (!bDidItWork)
	{
		UE_LOG(Equipment_Manager_Log, Warning,
			TEXT("EquipmentManagerCompnent::TriggerInput_Reload-> CurrentEquipment does not implement IWeaponInterface"));
	}
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
