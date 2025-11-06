// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/EquipmentData.h"
#include "Item/QuickSlotsBase.h"// why this one only works when it is included, not just forward declare
#include "Item/ChildClassesOfQuickSlots.h"

#include "EquipmentManagerComponent.generated.h"

class UInventoryManagerComponent;
class AWeaponBase;
class AItemBase;
class UEquipmentSlot;
class UInputActionHandler;
struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();
	
protected:
	// confirmations

	EEquipmentType CurrentEquipmentType=EEquipmentType::None;// default = none
	
	// Inventory
	UPROPERTY()
	UInventoryManagerComponent* InventoryComponent;// get this from player state
	UPROPERTY()
	TObjectPtr<UWeaponQuickSlots> WeaponQuickSlot;
	UPROPERTY()
	TObjectPtr<UItemQuickSlots> ItemQuickSlot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	AActor* CurrentEquipment;// the weapon or item that player character is currently holding

	
	// Placement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Placement")
	USceneComponent* Placement;// the Scene component of where to attach equipment

private:
	// quick slot switching
	bool bIsHoldingMouseRightButton;
	// this is for switching item quick slot.(hold rightmousebutton+ mouse wheel scroll=switch to next item)
	// if not, switch weapon
	
	/*//==== MouseWheel Tracking ====//
	// mouse wheele update ( to make it only work for once)
	FTimerHandle ScrollEndTimerHandle;// for setting timer
	
	float ScrollEndDelay=0.2f;

	float PreviousScrollSign=0.f;//+ or - or 0
	bool bIsScrolling=false;
	bool bDidScrollStarted=false;
	
	//for tracking holding to determine tap or hold
	float CurrentHoldingTime=0.0f;
	bool bDidHoldStarted=false;*/

	UPROPERTY()
	UInputActionHandler* EquipmentTriggerHelper;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;// for deleting the helper 
	//void SetPlacementComponent(USceneComponent* NewPlacement);// temporally put on public for testing

private:
	// control helper
	void OnRightMouseButtonPressed() {bIsHoldingMouseRightButton=true;}
	void OnRightMouseButtonReleased() {bIsHoldingMouseRightButton=false;}
	// use this to choose which quick slot to swtich
	
	//==== Inventory ====//
	void CacheInventoryComponent();
	
	void SwitchWeapon_PC_NumbKeys(uint8 NumbKeyValue);
	// helper function for sequential switching
	void GetNextEquipmentSlot(EEquipmentType Type, bool bIsDirectionRight);

	//helper for overall
	bool DoesTypeOfQuickSlotExist(EEquipmentType Type) const;
	bool SwitchCurrentEquipmentByType(EEquipmentType Type);
	bool AddQuickSlot(EEquipmentType Type, const FInitializeParams& Params);
	void ActivateOrDeactivateActor(AActor* Actor, bool bActivate);
	
	// this will handle visibility, Attachment setting and triggering equipment interface
	void EquipOrUnequip(AActor* EquipmentActor, bool bIsEquip);
	
	UEquipmentQuickSlots* GetQuickSlotByType(EEquipmentType Type) const;
	
public:
	// Temp For Weapon Test
	UFUNCTION(BlueprintCallable, Category = "Temp Testing")// to put the weapon in the character
	void SetCurrentEquipment(AActor* Equipment) {CurrentEquipment=Equipment;}

	UFUNCTION(BlueprintCallable, Category="Equipment")// this is to populate the quickslots from the inventory
	bool AddEquipmentFromInventory(int32 EquipmentID, EEquipmentType Type);
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SetCurrentEquipmentPlacement(USceneComponent* NewPlacement);// this will attatch the current weapon or item to the placementcomponent
	UFUNCTION()
	void SwitchWeapon_PC_MouseWheel(const FInputActionValue& Value);// will be done by mouse wheel scroll(one scroll== one switch)
	
	
	//==== Number Key Input Binding =====//
	UFUNCTION()
	void SwitchWeapon_PC_NumbKey1() { SwitchWeapon_PC_NumbKeys(1);}
	UFUNCTION()
	void SwitchWeapon_PC_NumbKey2() { SwitchWeapon_PC_NumbKeys(2);}
	UFUNCTION()
	void SwitchWeapon_PC_NumbLey3() { SwitchWeapon_PC_NumbKeys(3);}
	UFUNCTION()
	void SwitchWeapon_PC_NumbLey4() { SwitchWeapon_PC_NumbKeys(4);}
	UFUNCTION()
	void SwitchWeapon_PC_NumbLey5() { SwitchWeapon_PC_NumbKeys(5);}
	//----- So, the weapon quick slot max count is 5
	
	//===== GamePad Swtich =====//
	UFUNCTION()
	void SwitchWeapon_GP(const FInputActionValue& Value);
	/*UFUNCTION()
	void SwtichWeapon_GP_Next(const FInputActionValue& Value)// gamepad right face button
	{ GetNextEquipmentSlot(EEquipmentType::Weapon, true);}
	UFUNCTION()
	void SwtichWeapon_GP_Previous(const FInputActionValue& Value)// gamepad Left face button
	{ GetNextEquipmentSlot(EEquipmentType::Weapon, false);}*/
	UFUNCTION()
	void SwtichItem_GP_Next(const FInputActionValue& Value)
	{ GetNextEquipmentSlot(EEquipmentType::Item, true);}
	UFUNCTION()
	void SwtichItem_GP_Previous(const FInputActionValue& Value)
	{ GetNextEquipmentSlot(EEquipmentType::Item, false);}
	
	//---- Weapn and Item Interaction ----//----------------------------------------------------------------------------

	//Actually binded functions

	//-- basic input reactions
	UFUNCTION()
	void TriggerInput_Reload(const FInputActionValue& Value);


	//==== Trigger Input Helper
	UFUNCTION()
	void SetupTriggerHelper();
	
	UFUNCTION()
	void TriggerInput_Start(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Trigger(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Complete(const FInputActionValue& Value);
	/*UFUNCTION()
	void TriggerInput_Ongoing(const FInputActionValue& Value);*/
	UFUNCTION()
	void TriggerInput_Canceled(const FInputActionValue& Value);

	// Executing Interface here
	UFUNCTION()
	void TriggerTap();
	UFUNCTION()
	void TriggerHoldStart();
	UFUNCTION()
	void TriggerHoldUpdate(float Value);
	UFUNCTION()
	void TriggerReleased();
//----------------------------------------------------------------------------------------------------------------------

};