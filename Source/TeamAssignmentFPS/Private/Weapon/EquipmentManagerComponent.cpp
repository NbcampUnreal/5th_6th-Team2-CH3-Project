// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EquipmentManagerComponent.h"

#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "InputAction.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/EquipmentInterface.h"
#include "PlayerState/MyPlayerState.h"

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
		TEXT("UEquipmentManagerCompnent::SetPlacementComponent->Invaild USceneComp for Placement"));
		return;
	}

	Placement = NewPlacement;
}


void UEquipmentManagerComponent::OnScrollChunkStart(float ScrollDirection)
{
	if (ScrollDirection > 0)
	{
		UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerCompnent::OnScrollChunkStart->Scroll Start +"));
	}
	else
	{
		UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerCompnent::OnScrollChunkStart-> Scroll Start -"));
	}
		
}

void UEquipmentManagerComponent::OnScrollChunkStep(float ScrollDirection)
{
	/*FString Sign;
	if (PreviousScrollSign>0) Sign=TEXT("+");
	else if (PreviousScrollSign<0)Sign=TEXT("-");
	else Sign=TEXT("Zero");
	
	UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerCompnent::OnScrollChunkStep-> Switch happended %s"),*Sign);*/
	
}

void UEquipmentManagerComponent::OnScrollChunkEnd(float Direction)
{
	if (!bIsScrolling)
		return; // safety

	/*
	UE_LOG(Equipment_Manager_Log, Log, TEXT("[Scroll Chunk End] Direction: %s"),
		Direction > 0 ? TEXT("+") : TEXT("-"));*/

	bIsScrolling = false;
	bDidScrollStarted = false;
	PreviousScrollSign = 0.f;
}

void UEquipmentManagerComponent::ProcessScrollDetection(float ScrollDeltaValue, float DeltaTime)
{
	 
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

void UEquipmentManagerComponent::SpawnCurrentEquipment()
{
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerCompnent::SpawnCurrentEquipment-> Current Equipment is Invalid."));
		return;
	}

	const FTransform SpawnTransform=Placement->GetComponentTransform();
	
	AActor*SpawnedActor= GetWorld()->SpawnActor<AActor>(CurrentEquipment->GetClass(),SpawnTransform);
	if (!SpawnedActor)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentManagerCompnent::SpawnCurrentEquipment-> Equipment Spawing Failed."));
		return;
	}

	if (!SpawnedActor->Implements<UEquipmentInterface>())// when ther is no equipment interface
	{
		UE_LOG(Equipment_Manager_Log, Warning,
			TEXT("UEquipmentManagerCompnent::SpawnCurrentEquipment-> Current Equipment does not have equipment interface"));
		// but still use it, some could have no interface for calling equipped and unequipped
	}
	else// when there is equipment interface
	{
		IEquipmentInterface::Execute_OnEquipped(SpawnedActor);
	}

	//SpawnedActor->SetActorTransform(SpawnTransform);/--> no need, the spawn used the transform
	UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentManagerCompnent::SpawnCurrentEquipment-> CurrentEquipment is Set."));
}

void UEquipmentManagerComponent::TestEquipWeapon(AActor* SettingWeapon)
{
	SetCurrentEquipment(SettingWeapon);

	FVector PlacementLocation=Placement->GetComponentLocation();
	FRotator PlacementRotation=Placement->GetComponentRotation();
	
	SettingWeapon->SetActorLocationAndRotation(PlacementLocation, PlacementRotation);
}



void UEquipmentManagerComponent::UpdatePlacementComponent(USceneComponent* NewPlacement)
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


void UEquipmentManagerComponent::SetCurrentEquipmentPlacement()
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

void UEquipmentManagerComponent::SwtichWeapon_PC(const FInputActionValue& Value)
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
	float ScrollValue = Value.Get<float>();
	if (ScrollValue == 0.f)
		return;
	
	const float Sign = ScrollValue > 0.f ? 1.f : -1.f;


	//===  Handle direction change ===//
	if (bIsScrolling && Sign != PreviousScrollSign)
	{
		// End previous chunk immediately
		OnScrollChunkEnd(PreviousScrollSign);
		bIsScrolling = false;
		bDidScrollStarted = false;
	}

	//=== Handle new chunk start ===//
	if (!bIsScrolling)
	{
		OnScrollChunkStart(Sign);
		bIsScrolling = true;
		bDidScrollStarted = true;
		PreviousScrollSign = Sign;
	}

	//===  Restart timer for this chunk ===//
	GetWorld()->GetTimerManager().ClearTimer(ScrollEndTimerHandle);//reset timer
	//set new timer
	GetWorld()->GetTimerManager().SetTimer(
		ScrollEndTimerHandle,
		FTimerDelegate::CreateUObject(this, &UEquipmentManagerComponent::OnScrollChunkEnd, Sign),// make delegate to trigger function with delegate
		ScrollEndDelay,
		false
	);

	//===  Perform step logic (per scroll chunk movement) ===//
	OnScrollChunkStep(Sign);

}

void UEquipmentManagerComponent::SetCurrentEquipment(AActor* NewEquipment)
{
	CurrentEquipment=NewEquipment;
}

void UEquipmentManagerComponent::SwtichWeapon_GP(const FInputActionValue& Value)
{
	
}

void UEquipmentManagerComponent::SelectItem_PC(const FInputActionValue& Value)
{
	
}

void UEquipmentManagerComponent::SelectItem_GP(const FInputActionValue& Value)
{
	
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
			TEXT("UEquipmentManagerCompnent::TriggerInput_Reload-> not holding weapon currently."));

		// should this qutomatically switch to the weapon and reload the weapon?
		//TODO: decide the case. should this switch to weapon or do nothing and return
		return;
	}
	
	if (!CurrentEquipment)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("UEquipmentManagerCompnent::TriggerInput_Reload-> Invalid Equipment."));
		return;
	}

	if (!CurrentEquipment->Implements<UWeaponInterface>())// when the current equipment does not have the reload interface function
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("UEquipmentManagerCompnent::TriggerInput_Reload-> Invalid Equipment."));
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
