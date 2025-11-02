// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EquipmentManagerComponent.h"

#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "InputAction.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/EquipmentInterface.h"
#include "PlayerState/MyPlayerState.h"
#include "Item/EquipmentSlot.h"
#include "Debug/UELOGCategories.h"//debug log

UEquipmentManagerComponent::UEquipmentManagerComponent():
	CurrentEquipment(nullptr),
	Placement(nullptr)// where to put equipment
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	//bing inventory ptr to here
	CacheInventoryComponent();
}


void UEquipmentManagerComponent::CacheInventoryComponent()
{
	if (!GetOwner())
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Invalid Owner"));
		return;
	}

	AController* OwnerController=GetOwner()->GetInstigatorController();
	if (!OwnerController)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Invalid OwnerController"));
		return;
	}

	
	AMyPlayerState* PlayerState=OwnerController->GetPlayerState<AMyPlayerState>();// get the player state from the controller
	if (!PlayerState)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Invalid PlayerState"));
		return;
	}
	
	InventoryCompoent=PlayerState->GetInventoryManager();

	if (!InventoryCompoent)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Cached Invalid InventoryComponent"));
		return;
	}

	UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Cached Completed"));
}

void UEquipmentManagerComponent::SetPlacementComponent(USceneComponent* NewPlacement)
{
	if (!NewPlacement)
	{
		UE_LOG(Equipment_Manager_Log, Error,
		TEXT("UEquipmentManagerComponent::SetPlacementComponent->InVaild USceneComp for Placement"));
		return;
	}

	Placement = NewPlacement;
}

void UEquipmentManagerComponent::SpawnEquipmentInSlot(int32 ID,  EEquipmentType Type,TMap<int32, TObjectPtr<AActor>>& Slot)
{
	if (!InventoryCompoent)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SpawnEquipmentInSlot-> Invalid Inventory comp"));
		return;
	}

	

	//TSubclassOf<AActor> EquipmentClass=InventoryCompoent->
}

void UEquipmentManagerComponent::SwtichWeapon_PC_NumbKeys(uint8 NumbKeyValue)
{
	UEquipmentQuickSlots* WeaponQuickSlot = *EquipmentSlotsList.Find(EEquipmentType::Weapon);
	if (!WeaponQuickSlot)
	{
		UE_LOG(Equipment_Manager_Log, Error,
		TEXT(" UEquipmentManagerComponent::SwtichWeapon_PC_NumbKeys-> Cannot Find WeaponQuickSlot."));
		// do nothing here.this is real problem
		return;
	}
	//WeaponQuickSlot found

	if (!WeaponQuickSlot->SwitchCurrentSlot(NumbKeyValue))
	{
		UE_LOG(Equipment_Manager_Log, Warning,
		TEXT(" UEquipmentManagerComponent::SwtichWeapon_PC_NumbKeys-> %d has no available weapon.")
		,NumbKeyValue);
		// TODO: trigger slot widget to play error effect
		return;
	}

	CurrentEquipment=WeaponQuickSlot->GetCurrentEquipmentPtr();
	UE_LOG(Equipment_Manager_Log, Log,
		TEXT(" UEquipmentManagerComponent::SwtichWeapon_PC_NumbKeys-> %d Weapon Setting is completed.")
		,NumbKeyValue);
	SetCurrentEquipmentPlacement();// set location to the hodling location; just in case
}

void UEquipmentManagerComponent::GetNextEquipmentSlot(EEquipmentType Type, bool bIsDirectionRight)
{
	if (!DoesTypeOfQuickSlotExist(Type)) return;

	UEquipmentQuickSlots* QuickSlots = EquipmentSlotsList[Type];
	if (!QuickSlots)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::GetNextEquipmentSlot-> QuickSlots pointer is Invalid"));
		return;
	}
	if (!QuickSlots->SwitchToNextSlot(bIsDirectionRight))
	{
		UE_LOG(Equipment_Manager_Log, Warning,
			TEXT("UEquipmentManagerComponent::GetNextEquipmentSlot-> Failed to switch slot"));
		return;
	}
	
	SwitchCurrentEquipmentByType(Type);
	// TODO: send signal to widget for update
	// no need to , cause the slot it self will send signal
}

bool UEquipmentManagerComponent::DoesTypeOfQuickSlotExist(EEquipmentType Type) const
{
	FString QuickSlotType=UEnum::GetValueAsString(Type);
	
	if (!EquipmentSlotsList.Contains(Type))
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::GetNextEquipmentSlot-> Cannot find %s from list")
			, *QuickSlotType);
		return false;
	}
	
	UE_LOG(Equipment_Manager_Log, Log,
	TEXT("UEquipmentManagerComponent::GetNextEquipmentSlot-> %s got found from list")
	, *QuickSlotType);
	return true;
}

bool UEquipmentManagerComponent::SwitchCurrentEquipmentByType(EEquipmentType Type)
{
	if (!DoesTypeOfQuickSlotExist(Type))
		return false;

	UEquipmentQuickSlots* QuickSlots = EquipmentSlotsList[Type];
	if (!QuickSlots)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SwitchCurrentEquipmentByType -> Invalid QuickSlot for %s"),
			*UEnum::GetValueAsString(Type));// wow fuck yeah. now can print enum display name
		return false;
	}

	AActor* NewEquipment = QuickSlots->GetCurrentEquipmentPtr();
	if (!NewEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SwitchCurrentEquipmentByType -> No valid equipment in current slot for %s"),
			*UEnum::GetValueAsString(Type));
		return false;
	}
	
	// unequip the soon be replaced equipment if it has interface
	if (CurrentEquipment && CurrentEquipment->Implements<UEquipmentInterface>())
	{
		IEquipmentInterface::Execute_OnUnequipped(CurrentEquipment);
	}

	CurrentEquipmentType = Type;
	CurrentEquipment = NewEquipment;
	SetCurrentEquipmentPlacement();

	// trigger equip using interface
	if (CurrentEquipment->Implements<UEquipmentInterface>())
	{
		IEquipmentInterface::Execute_OnEquipped(CurrentEquipment);
	}

	UE_LOG(Equipment_Manager_Log, Log,
		TEXT("UEquipmentManagerComponent::SwitchCurrentEquipmentByType -> Switched to %s"),
		*UEnum::GetValueAsString(Type));

	return true;
}


void UEquipmentManagerComponent::SpawnCurrentEquipment()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SpawnCurrentEquipment-> Current Equipment is Invalid."));
		return;
	}

	const FTransform SpawnTransform=Placement->GetComponentTransform();
	
	AActor*SpawnedActor= GetWorld()->SpawnActor<AActor>(CurrentEquipment->GetClass(),SpawnTransform);
	if (!SpawnedActor)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SpawnCurrentEquipment-> Equipment Spawing Failed."));
		return;
	}

	if (!SpawnedActor->Implements<UEquipmentInterface>())// when ther is no equipment interface
	{
		UE_LOG(Equipment_Manager_Log, Warning,
			TEXT("UEquipmentManagerComponent::SpawnCurrentEquipment-> Current Equipment does not have equipment interface"));
		// but still use it, some could have no interface for calling equipped and unequipped
	}
	else// when there is equipment interface
	{
		IEquipmentInterface::Execute_OnEquipped(SpawnedActor);
	}

	//SpawnedActor->SetActorTransform(SpawnTransform);/--> no need, the spawn used the transform
	UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerComponent::SpawnCurrentEquipment-> CurrentEquipment is Set."));
}



void UEquipmentManagerComponent::UpdatePlacementComponent(USceneComponent* NewPlacement)
{
	if (!NewPlacement)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::UpdatePlacementComponent-> Invalid Placedment"));
		return;
	}
	
	Placement=NewPlacement;
	SetCurrentEquipmentPlacement();
}


void UEquipmentManagerComponent::SetCurrentEquipmentPlacement()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SetCurrentEquipmentPlacement-> no equipment to place"));
		return;
	}
	if (!Placement)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SetCurrentEquipmentPlacement-> no place to equip"));
		return;
	}

	CurrentEquipment->AttachToComponent(Placement, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CurrentEquipment->SetActorRelativeLocation(FVector::ZeroVector);//offset
	CurrentEquipment->SetActorRelativeRotation(FRotator::ZeroRotator);//offset
}

void UEquipmentManagerComponent::SwtichWeapon_PC_MouseWheel(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollValue))//not enough scroll movement
		return;
	
	const bool bScrollUp = (ScrollValue > 0.f);

	EEquipmentType TargetType=bIsHoldingMouseRightButton? EEquipmentType::Item:EEquipmentType::Weapon;
	GetNextEquipmentSlot(TargetType,bScrollUp);
}

void UEquipmentManagerComponent::TriggerInput_Reload(const FInputActionValue& Value)
{
	/*if (!CurrentEquipment)
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
	}*/// need to check if current equipment type is weapon

	if (CurrentEquipmentType!=EEquipmentType::Weapon)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("UEquipmentManagerComponent::TriggerInput_Reload-> not holding weapon currently."));

		// should this qutomatically switch to the weapon and reload the weapon?
		//TODO: decide the case. should this switch to weapon or do nothing and return
		// swicth to weapon
		return;
	}
	
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("UEquipmentManagerComponent::TriggerInput_Reload-> Invalid Equipment."));
		return;
	}

	if (!CurrentEquipment->Implements<UWeaponInterface>())// when the current equipment does not have the reload interface function
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("UEquipmentManagerComponent::TriggerInput_Reload-> Invalid Equipment."));
		return;
	}

	//chekcing completed
	IWeaponInterface::Execute_OnReloadInputPressed(CurrentEquipment);
}

void UEquipmentManagerComponent::TriggerInput_Start(const FInputActionValue& Value)
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerInput_Start-> CurrentEquipment is invalid"));
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
			TEXT("UEquipmentManagerComponent::TriggerInput_Start-> CurrentEquipment does not have required interface"));
		// TODO:
		// siganl ui that equipment trigger is invalid (ex. red highlight for current equipment widget)
		// player character playing anim for invalid command?
	}
		
}

void UEquipmentManagerComponent::TriggerInput_Trigger(const FInputActionValue& Value)
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

void UEquipmentManagerComponent::TriggerInput_Complete(const FInputActionValue& Value)
{
	if (!CurrentEquipment) return;

	float TapThreshold =0.2f;
	FInputTypeHelper::HandleTapOrHoldRelease(CurrentEquipment, CurrentHoldingTime,TapThreshold);

	// Reset the variables
	CurrentHoldingTime = 0.f;
	bDidHoldStarted = false;
}

void UEquipmentManagerComponent::TriggerInput_Ongoing(const FInputActionValue& Value)
{
	// not decided yet. maybe update the ui?
}

void UEquipmentManagerComponent::TriggerInput_Canceled(const FInputActionValue& Value)// not so sure where to use this
{
	if (!CurrentEquipment)
	{
		//error
		return;
	}

	// should this be treated as a release or just cancle and don't trigger the function? not so sure fuck
	//IInputReactionInterface::Execute_OnInputRelease(CurrentEquipment);

	//cancled situation needs to be seperate from the completed case. just dont finish the prior process

	CurrentHoldingTime = 0.f;
	bDidHoldStarted = false;
}
