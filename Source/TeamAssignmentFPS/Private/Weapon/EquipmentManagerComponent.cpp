// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EquipmentManagerComponent.h"

#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "InputAction.h"
#include "Character/MyCharacter.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/EquipmentInterface.h"
#include "PlayerState/MyPlayerState.h"
#include "Item/EquipmentSlot.h"
#include "Item/InventoryManagerComponent.h"
#include "InputHelper/InputActionHandler.h"//for input action helper
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
	// equipment trigger input helper
	SetupTriggerHelper();

}

void UEquipmentManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EquipmentTriggerHelper)
	{
		EquipmentTriggerHelper->RemoveFromRoot();
		EquipmentTriggerHelper=nullptr;
	}
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
	
	InventoryComponent=PlayerState->GetInventoryManager();

	if (!InventoryComponent)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Cached Invalid InventoryComponent"));
		return;
	}

	UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::CacheInventoryComponent-> Cached Completed"));
}


void UEquipmentManagerComponent::SwitchWeapon_PC_NumbKeys(uint8 NumbKeyValue)
{
	if (!WeaponQuickSlot)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SwtichWeapon_PC_NumbKeys -> WeaponQuickSlot is invalid"));
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
	SetCurrentEquipmentPlacement(Placement);// set location to the hodling location; just in case
}

void UEquipmentManagerComponent::GetNextEquipmentSlot(EEquipmentType Type, bool bIsDirectionRight)
{
	UEquipmentQuickSlots* QuickSlot = GetQuickSlotByType(Type);
	if (!QuickSlot)
		return;
	
	if (!QuickSlot->SwitchToNextSlot(bIsDirectionRight))
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
	bool bExists = false;

	switch (Type)
	{
	case EEquipmentType::Weapon:
		bExists = (WeaponQuickSlot != nullptr);
		break;

	case EEquipmentType::Item:
		bExists = (ItemQuickSlot != nullptr);
		break;

	default:
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::DoesTypeOfQuickSlotExist -> Invalid EquipmentType"));
		return false;
	}

	FString QuickSlotTypeStr = UEnum::GetValueAsString(Type);

	if (!bExists)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::DoesTypeOfQuickSlotExist -> %s QuickSlot does not exist"), 
			*QuickSlotTypeStr);
	}
	else
	{
		UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerComponent::DoesTypeOfQuickSlotExist -> %s QuickSlot exists"), 
			*QuickSlotTypeStr);
	}

	return bExists;
}

bool UEquipmentManagerComponent::SwitchCurrentEquipmentByType(EEquipmentType Type)
{
	if (!DoesTypeOfQuickSlotExist(Type))
		return false;

	UEquipmentQuickSlots* QuickSlot = GetQuickSlotByType(Type);
	if (!QuickSlot)
		return false;

	AActor* NewEquipment = QuickSlot->GetCurrentEquipmentPtr();
	if (!NewEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SwitchCurrentEquipmentByType -> No valid equipment in current slot for %s"),
			*UEnum::GetValueAsString(Type));
		return false;
	}

	// unequip the current one for new one
	if (CurrentEquipment)
	{
		EquipOrUnequip(CurrentEquipment, false);
	}
	// switch to new one
	CurrentEquipment = NewEquipment;
	CurrentEquipmentType = Type;

	EquipOrUnequip(CurrentEquipment, true);
	return true;
}

bool UEquipmentManagerComponent::AddQuickSlot(EEquipmentType Type, const FInitializeParams& Params)
{
	UEquipmentQuickSlots* QuickSlots = nullptr;

	switch (Type)
	{
	case EEquipmentType::Weapon:
		QuickSlots = WeaponQuickSlot;
		break;
	case EEquipmentType::Item:
		QuickSlots = ItemQuickSlot;
		break;
	default:
		UE_LOG(Equipment_Manager_Log, Error, 
			TEXT("UEquipmentManagerComponent::AddQuickSlot -> Invalid EquipmentType"));
		return false;
	}

	if (!QuickSlots)
	{
		QuickSlots = NewObject<UEquipmentQuickSlots>(this);

		switch (Type)
		{
		case EEquipmentType::Weapon:
			WeaponQuickSlot = Cast<UWeaponQuickSlots>(QuickSlots);
			break;
		case EEquipmentType::Item:
			ItemQuickSlot = Cast<UItemQuickSlots>(QuickSlots);
			break;
		default:
			UE_LOG(Equipment_Manager_Log, Error, 
				TEXT("UEquipmentManagerComponent::AddQuickSlot -> Invalid EquipmentType"));
			return false;
		}
	}

	if (!QuickSlots->AddEquipment(Params))
	{
		UE_LOG(Equipment_Manager_Log, Warning,
			TEXT("UEquipmentManagerComponent::AddQuickSlot -> Failed to add equipment ID %d"), Params.ID);
		return false;
	}

	// Activate the equipment if nothing is equipped
	if (!CurrentEquipment)
	{
		CurrentEquipment = Params.Equipment;
		CurrentEquipmentType = Type;
		EquipOrUnequip(CurrentEquipment, true);
	}

	return true;
}
void UEquipmentManagerComponent::ActivateOrDeactivateActor(AActor* Actor, bool bActivate)// same as the quick slot base function
{
	if (!Actor) return;

	// Visibility
	Actor->SetActorHiddenInGame(!bActivate);

	// Collision
	Actor->SetActorEnableCollision(bActivate);

	// Ticking
	Actor->SetActorTickEnabled(bActivate);

	// Physics / primitive components
	TArray<UPrimitiveComponent*> Components;
	Actor->GetComponents<UPrimitiveComponent>(Components);
	for (UPrimitiveComponent* Comp : Components)
	{
		if (Comp)
		{
			Comp->SetSimulatePhysics(bActivate && Comp->IsSimulatingPhysics());
			Comp->SetCollisionEnabled(bActivate ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
		}
	}
}

void UEquipmentManagerComponent::EquipOrUnequip(AActor* EquipmentActor, bool bIsEquip)
{
	if (!EquipmentActor)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::EquipOrUnequip->Invalid EquipmentActor"));
		return;
	}

	ActivateOrDeactivateActor(EquipmentActor, bIsEquip);

	// Trigger interface
	if (EquipmentActor->Implements<UEquipmentInterface>())
	{
		if (bIsEquip)
			IEquipmentInterface::Execute_OnEquipped(EquipmentActor);
		else
			IEquipmentInterface::Execute_OnUnequipped(EquipmentActor);
	}

	if (bIsEquip)
	{
		SetCurrentEquipmentPlacement(Placement);
	}
}

UEquipmentQuickSlots* UEquipmentManagerComponent::GetQuickSlotByType(EEquipmentType Type) const
{
	UEquipmentQuickSlots* QuickSlotPtr = nullptr;

	switch (Type)
	{
	case EEquipmentType::Weapon:
		QuickSlotPtr = WeaponQuickSlot;
		break;
	case EEquipmentType::Item:
		QuickSlotPtr = ItemQuickSlot;
		break;
	default:
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::GetQuickSlotByType -> Invalid EquipmentType"));
		return nullptr;
	}

	if (!QuickSlotPtr)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::GetQuickSlotByType -> QuickSlot is null for %s"),
			*UEnum::GetValueAsString(Type));
		return nullptr;
	}

	return QuickSlotPtr;
}

bool UEquipmentManagerComponent::PutActorIntoSlot(uint8 SlotIndex, EEquipmentType SlotType, AActor* NewEquipment)
{
	TMap<uint8, AActor*>* EquipmentMap=nullptr;
	switch (SlotType)
	{
	case EEquipmentType::Weapon:
		EquipmentMap=&TempWeaponQuickSlot;
		break;
	case EEquipmentType::Item:
		EquipmentMap=&TempItemQuickSlot;
		break;
	default:
		return false;
	}

	if (!EquipmentMap)
	{
		//invalid
		return false;
	}

	EquipmentMap->Add(SlotIndex,NewEquipment);
	return true;
}

void UEquipmentManagerComponent::ActivateOrDeactivate(bool bIsEquip, AActor* Equipment)
{
	if (!Equipment)
	{
		//error, no valid equipment to equip or unequip
		return;
	}

	Equipment->SetActorHiddenInGame(!bIsEquip);
	Equipment->SetActorEnableCollision(bIsEquip);
	Equipment->SetActorTickEnabled(bIsEquip);

	UE_LOG(Equipment_Manager_Log, Log,
		TEXT("UEquipmentManagerComponent::EquipOrUnequip-> New Equipment %s is %s"),
		*Equipment->GetName(),*FString((bIsEquip)?TEXT("Equipped"):TEXT("Unequipped")));
	
}

void UEquipmentManagerComponent::EquipCurrentEquipment(AActor* NewEquipment)
{
	if (!NewEquipment)
	{
		//error, invalid new equipment. equipment failed
		return;
	}
	
	//Switch
	//previous equipment-> unequip
	AActor* PreviousEquipment=CurrentEquipment;
	if (!PreviousEquipment) return;// invalid, just return
	if (!PreviousEquipment->Implements<UEquipmentInterface>()) return;// no equipment just deactivate
	
	IEquipmentInterface::Execute_OnUnequipped(PreviousEquipment);
	
	// new current equipment-> equip
	CurrentEquipment=NewEquipment;
	if (!CurrentEquipment) return;
	if (!CurrentEquipment->Implements<UEquipmentInterface>()) return;
	
	IEquipmentInterface::Execute_OnEquipped(CurrentEquipment);
}

void UEquipmentManagerComponent::SwitchToNextSlot(bool bIsRight, TMap<uint8, AActor*>& QuickSlot,
	uint8 CurrentSlotIndex)
{
	if (QuickSlot.IsEmpty())
	{
		return;
	}

	TArray<uint8> SlotIndices;
	QuickSlot.GetKeys(SlotIndices);
	SlotIndices.Sort();

	uint8 NextSlotIndex=CurrentSlotIndex;
	int32 SlotCount=SlotIndices.Num();
	
	for (uint8 i=0;i<SlotCount;i++)
	{
		if (bIsRight)
		{
			NextSlotIndex=(NextSlotIndex+1)%SlotCount;// so that remaining becomes the first
		}
		else
		{
			NextSlotIndex=(NextSlotIndex-1)%SlotCount;
		}

		uint8 NextSlotIndexIndex=SlotIndices[NextSlotIndex];
		AActor* NextEquipment= QuickSlot[NextSlotIndexIndex];

		if (!NextEquipment) continue;

		EquipCurrentEquipment(NextEquipment);
	}
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 2966a65 (equipment updated)
void UEquipmentManagerComponent::SwitchFromItemToWeapon()
{
	AActor* CurrentWeapon= TempWeaponQuickSlot[CurrentWeaponSlotIndex];// bring the current weapon from current index

	EquipCurrentEquipment(CurrentWeapon);
	//
}
<<<<<<< HEAD
=======
>>>>>>> 6bfcd5b (update)
=======
>>>>>>> 2966a65 (equipment updated)
=======
>>>>>>> cccbfc2 (Revert "murge into seo")


bool UEquipmentManagerComponent::AddEquipmentFromInventory(int32 EquipmentID, EEquipmentType Type)
{
	if (!InventoryComponent)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::AddEquipmentFromInventory -> Invalid InventoryComponent"));
		return false;
	}

	switch (Type)
	{
	case EEquipmentType::Weapon:
	{
		TMap<int32, FWeaponData>* Weapons = InventoryComponent->GetWeaponsPtr();
		if (!Weapons)
		{
			UE_LOG(Equipment_Manager_Log, Error,
				TEXT("UEquipmentManagerComponent::AddEquipmentFromInventory -> Invalid Weapon Map"));
			return false;
		}

		FWeaponData* FoundData = Weapons->Find(EquipmentID);
		if (!FoundData || !FoundData->EquipmentClass)
		{
			UE_LOG(Equipment_Manager_Log, Warning,
				TEXT("Weapon ID %d not found or has invalid EquipmentClass"), EquipmentID);
			return false;
		}

		if (!WeaponQuickSlot)
			WeaponQuickSlot = NewObject<UWeaponQuickSlots>(this);

		// let the quickslot handle the spawn
		if (!WeaponQuickSlot->InitializeWeaponSlot(EquipmentID, FoundData->EquipmentClass))
		{
			UE_LOG(Equipment_Manager_Log, Warning,
				TEXT("Failed to initialize WeaponQuickSlot for EquipmentID %d"), EquipmentID);
			return false;
		}

		break;
	}

	case EEquipmentType::Item:
	{
		TMap<int32, FItemData>* Items = InventoryComponent->GetItemsPtr();
		if (!Items)
		{
			UE_LOG(Equipment_Manager_Log, Error,
				TEXT("UEquipmentManagerComponent::AddEquipmentFromInventory -> Invalid Item Map"));
			return false;
		}

		FItemData* FoundData = Items->Find(EquipmentID);
		if (!FoundData || !FoundData->EquipmentClass)
		{
			UE_LOG(Equipment_Manager_Log, Warning,
				TEXT("Item ID %d not found or has invalid EquipmentClass"), EquipmentID);
			return false;
		}

		if (!ItemQuickSlot)
			ItemQuickSlot = NewObject<UItemQuickSlots>(this);

		// let the quickslot handle the spawn
		if (!ItemQuickSlot->InitializeItemSlot(EquipmentID, FoundData->EquipmentClass, FoundData->SlotStackMaxCount))
		{
			UE_LOG(Equipment_Manager_Log, Warning,
				TEXT("Failed to initialize ItemQuickSlot for EquipmentID %d"), EquipmentID);
			return false;
		}

		break;
	}

	default:
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::AddEquipmentFromInventory -> Invalid EquipmentType"));
		return false;
	}

	return true;
}


void UEquipmentManagerComponent::SetCurrentEquipmentPlacement(USceneComponent* NewPlacement)
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SetCurrentEquipmentPlacement-> no equipment to place"));
		return;
	}
	if (!NewPlacement)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::SetCurrentEquipmentPlacement-> Invalid NewPlacement"));
		return;
	}

	Placement = NewPlacement;

	CurrentEquipment->AttachToComponent(Placement, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CurrentEquipment->SetActorRelativeLocation(FVector::ZeroVector);//offset
	CurrentEquipment->SetActorRelativeRotation(FRotator::ZeroRotator);//offset
	UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerComponent::SetCurrentEquipmentPlacement-> Placement Setting Completed"));
}

void UEquipmentManagerComponent::SwitchWeapon_PC_MouseWheel(const FInputActionValue& Value)
{
	float ScrollValue = Value.Get<float>();
	if (FMath::IsNearlyZero(ScrollValue))//not enough scroll movement
		return;
	
	const bool bScrollUp = (ScrollValue > 0.f);

	EEquipmentType TargetType=bIsHoldingMouseRightButton? EEquipmentType::Item:EEquipmentType::Weapon;
	GetNextEquipmentSlot(TargetType,bScrollUp);
}

void UEquipmentManagerComponent::SwitchWeapon_GP(const FInputActionValue& Value)
{
	float InputValue = Value.Get<float>();
	bool bIsNextOrPrevious =( InputValue > 0);

	GetNextEquipmentSlot(EEquipmentType::Weapon, bIsNextOrPrevious);
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

	/*if (CurrentEquipmentType!=EEquipmentType::Weapon)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("UEquipmentManagerComponent::TriggerInput_Reload-> not holding weapon currently."));

		// should this qutomatically switch to the weapon and reload the weapon?
		//TODO: decide the case. should this switch to weapon or do nothing and return
		// swicth to weapon
		return;
	}*/
	
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

void UEquipmentManagerComponent::SetupTriggerHelper()
{
	EquipmentTriggerHelper=NewObject<UInputActionHandler>(this, UInputActionHandler::StaticClass());
	if (!EquipmentTriggerHelper)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT(" UEquipmentManagerComponent::SetupTriggerHelper-> Invalid EquipmentTriggerHelper."));
		return;
	}

	EquipmentTriggerHelper->AddToRoot();//for safety--> need to remove when leave

	EquipmentTriggerHelper->SetShouldTriggerWhenCanceled(false);// do nothing when the trigger is canceled

	EquipmentTriggerHelper->OnTapped.BindUObject(this, &UEquipmentManagerComponent::TriggerTap);
	EquipmentTriggerHelper->OnHoldStart.BindUObject(this, &UEquipmentManagerComponent::TriggerHoldStart);
	EquipmentTriggerHelper->OnHoldUpdate_Float.BindUObject(this, &UEquipmentManagerComponent::TriggerHoldUpdate);
	EquipmentTriggerHelper->OnReleased.BindUObject(this, &UEquipmentManagerComponent::TriggerReleased);

	UE_LOG(Equipment_Manager_Log,Log,
	TEXT(" UEquipmentManagerComponent::SetupTriggerHelper-> Helper Bound complete."));
	
}

void UEquipmentManagerComponent::TriggerInput_Start(const FInputActionValue& Value)
{
}

void UEquipmentManagerComponent::TriggerInput_Trigger(const FInputActionValue& Value)
{
	if (!EquipmentTriggerHelper) return;
	float FloatValue = Value.Get<float>();
	EquipmentTriggerHelper->OnTriggerPressed(FloatValue);
}

void UEquipmentManagerComponent::TriggerInput_Complete(const FInputActionValue& Value)
{
	if (!EquipmentTriggerHelper) return;
	EquipmentTriggerHelper->OnTriggerCompleted();
}


void UEquipmentManagerComponent::TriggerInput_Canceled(const FInputActionValue& Value)// not so sure where to use this
{
	if (!EquipmentTriggerHelper) return;
	EquipmentTriggerHelper->OnTriggerCanceled();
}

void UEquipmentManagerComponent::TriggerTap()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerTap-> Invalid Current Equipment"));
		return;
	}
	if (!CurrentEquipment->Implements<UInputReactionInterface>())
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerTap-> Current Equipment has no Tap"));
		return;
	}
	
	IInputReactionInterface::Execute_OnInputTap(CurrentEquipment);
	UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerComponent::TriggerTap->Tap"));
	
}

void UEquipmentManagerComponent::TriggerHoldStart()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerHoldStart-> Invalid Current Equipment"));
		return;
	}
	if (!CurrentEquipment->Implements<UInputReactionInterface>())
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerHoldStart-> Current Equipment has no HoldStart"));
		return;
	}
	IInputReactionInterface::Execute_OnInputHoldStart(CurrentEquipment);
	UE_LOG(Equipment_Manager_Log, Log,
		TEXT("UEquipmentManagerComponent::TriggerHoldStart-> HoldStart"));
}

void UEquipmentManagerComponent::TriggerHoldUpdate(float Value)
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerHoldUpdate-> Invalid Current Equipment"));
		return;
	}
	if (!CurrentEquipment->Implements<UInputReactionInterface>())
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerHoldUpdate-> Current Equipment has no HoldUpdate"));
		return;
	}
	IInputReactionInterface::Execute_OnInputHoldUpdate(CurrentEquipment, Value);
	UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerComponent::TriggerHoldUpdate->Hold Update"));
}

void UEquipmentManagerComponent::TriggerReleased()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerReleased-> Invalid Current Release"));
		return;
	}
	if (!CurrentEquipment->Implements<UInputReactionInterface>())
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerComponent::TriggerReleased-> Current Equipment has no Release"));
		return;
	}
	IInputReactionInterface::Execute_OnInputRelease(CurrentEquipment);
	UE_LOG(Equipment_Manager_Log, Log,
	TEXT("UEquipmentManagerComponent::TriggerReleased-> Hold Released"));
}
