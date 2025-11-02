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
	UInventoryManagerComponent* InventoryCompoent;// get this from player state
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	bool bIsEquipping;//is current equipment empty or not*///--> no need, just check CurrentEquipment

	//==== Quick Slot ====//
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSlot|Weapon")
	TMap<int32/*ID#1#,TObjectPtr<AActor>/*Weapon#1#> WeaponQuickSlot;
	// the id will be shared with the inventory. the information of the weapon will be aquired from the inventory
	int32 CurrentWeaponID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSlot|Item")
	TMap<int32/*ID#1#,TObjectPtr<AActor>/*Item#1#> ItemQuickSlot;
	// the id will be shared with the inventory. the information of the weapon will be aquired from the inventory
	int32 CurrentItemID;*/

	TMap<EEquipmentType, UEquipmentQuickSlots> EquipmentSlotsList;// for weapons, items and more in the future

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
	
	//==== MouseWheel Tracking ====//
	
	// mouse wheele update ( to make it only work for once)
	FTimerHandle ScrollEndTimerHandle;// for setting timer
	
	float ScrollEndDelay=0.2f;

	float PreviousScrollSign=0.f;//+ or - or 0
	bool bIsScrolling=false;
	bool bDidScrollStarted=false;


	//for tracking holding to determine tap or hold
	float CurrentHoldingTime=0.0f;
	bool bDidHoldStarted=false;
	

	//Test Temp

	// for actually showing up the weapon or the item
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SpawnCurrentEquipment();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//void SetPlacementComponent(USceneComponent* NewPlacement);// temporally put on public for testing

private:
	// control helper
	void OnRightMouseButtonPressed() {bIsHoldingMouseRightButton=true;}
	void OnRightMouseButtonReleased() {bIsHoldingMouseRightButton=false;}
	// use this to choose which quick slot to swtich
	
	//==== Inventory ====//
	void CacheInventoryComponent();
	
	//== Mouse scroll action
	void OnScrollChunkStart(float ScrollDirection);
	void OnScrollChunkStep(float ScrollDirection);
	void OnScrollChunkEnd(float ScrollDirection);
	void ProcessScrollDetection(float ScrollDltaValue, float DeltaTime);


	// Equipment setting
	void SpawnEquipmentInSlot(int32 ID,  EEquipmentType Type,TMap<int32, TObjectPtr<AActor>>& Slot);
	
	void SwtichWeapon_PC_NumbKeys(uint8 NumbKeyValue);
public:

	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SetPlacementComponent(USceneComponent* NewPlacement);

	//Temp for controlling weapon without inventory( weapon in the editor )
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void TestEquipWeapon(AActor* SettingWeapon);
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void UpdatePlacementComponent(USceneComponent* NewPlacement);// this will update where to be attatched
	void SetCurrentEquipmentPlacement();// this will attatch the current weapon or item to the placementcomponent
	UFUNCTION()
	void SwtichWeapon_PC_MouseWheel(const FInputActionValue& Value);// will be done by mouse wheel scroll(one scroll== one switch)

	//==== Number Key Input Binding =====//
	UFUNCTION()
	void SwtichWeapon_PC_Numbkey1() { SwtichWeapon_PC_NumbKeys(1);}
	UFUNCTION()
	void SwtichWeapon_PC_Numbkey2() { SwtichWeapon_PC_NumbKeys(2);}
	UFUNCTION()
	void SwtichWeapon_PC_Numbkey3() { SwtichWeapon_PC_NumbKeys(3);}
	UFUNCTION()
	void SwtichWeapon_PC_Numbkey4() { SwtichWeapon_PC_NumbKeys(4);}
	UFUNCTION()
	void SwtichWeapon_PC_Numbkey5() { SwtichWeapon_PC_NumbKeys(5);}
	//----- So, the weapon quick slot max count is 5
	
	
	UFUNCTION()
	void SwtichWeapon_GP(const FInputActionValue& Value);
	void SetCurrentEquipment(AActor* NewEquipment);
	UFUNCTION()
	void SelectItem_PC(const FInputActionValue& Value);//
	UFUNCTION()
	void SelectItem_GP(const FInputActionValue& Value);//
	


	//---- Weapn and Item Interaction ----//----------------------------------------------------------------------------

	//Actually binded functions

	//-- basic input reactions
	UFUNCTION()
	void TriggerInput_Reload(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Start(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Trigger(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Complete(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Ongoing(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerInput_Canceled(const FInputActionValue& Value);
//----------------------------------------------------------------------------------------------------------------------

};