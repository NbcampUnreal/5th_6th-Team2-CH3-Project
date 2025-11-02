// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"
#include "Item/EquipmentData.h"

#include "EquipmentManagerComponent.generated.h"

class UInventoryManagerComponent;
class AWeaponBase;
class AItemBase;

struct FInputActionValue;


USTRUCT(BlueprintType)
struct FEquipmentSlot// one slot
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<AActor> EquipmentPtr=nullptr;

private:
	UPROPERTY()
	int32 EquipmentID=INDEX_NONE;

public:
	bool InitializeEquipmentSlot(AActor* Equipment,int32 ID);

	int32 GetEquipmentID()const {return EquipmentID;}
	AActor* GetEquipmentPtr() const {return EquipmentPtr;}
};

// make Child class of slots (Weapon, Items)

USTRUCT(BlueprintType)
struct FWeaponSlot:public FEquipmentSlot// one slot
{
	GENERATED_BODY()

	/*
	 TODO: put required infos in here for weapon

	 */
	
protected:
	bool InitializeWeaponData(AActor* Equipment,int32 ID);
};

USTRUCT(BlueprintType)
struct FItemSlot:public FEquipmentSlot// one slot
{
	GENERATED_BODY()
	
private:
	int32 MAxStackCount=0;
	int32 CurrentStackCount=INDEX_NONE;

public:
	int32 GetCurrentStackCount()const {return MAxStackCount;}
	int32 GetMaxStackCount()const {return CurrentStackCount;}

	void IncrementStackCount();// only increment and decrement with 1 cause the leftover case happens
	void DecrementStackCount();

	
	bool InitializeItemData(AActor* Equipment,int32 ID, int32 MaxStackCount);
};




// slot struct to manage equipments
USTRUCT(BlueprintType)
struct FEquipmentQuickSlots//multiple slots
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment Slot")
	TMap<uint8/*slot Number*/,TObjectPtr<AActor>/*Equipment*/> EquipmentQuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment Slot")
	int32 SlotMaxCount;
	

private:
	
	int32 CurrentEquipmentID=INDEX_NONE;//default
	int32 UsedSlotCount=0;
	uint8 CurrentSlotIndex=1;// the main
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot")
	AActor* CurrentSlotEquipment=nullptr;// not currently equipt by player but equipment of current slot index
	
public:
	AActor* GetEquipmentFromCurrentSlot() const { return CurrentSlotEquipment;}
	uint8 GetCurrentSlotIndex() const { return CurrentSlotIndex; }
	int32 GetCurrentEquipmentID() const { return CurrentEquipmentID;}

	bool AddToSlot(uint8 SlotIndex, TObjectPtr<AActor> Equipment);
	bool RemoveFromSlot(uint8 SlotIndex);

};
//---------------------------------------------------------------------------------------------------------------------

//=========================<< UEquipmentManagerComponent >>============================================================//

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

	TMap<EEquipmentType, FEquipmentQuickSlots> EquipmentSlots;// for weapons, items and more in the future
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	AActor* CurrentEquipment;// the weapon or item that player character is currently holding

	
	// Placement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Placement")
	USceneComponent* Placement;// the Scene component of where to attach equipment




	
private:

	//==== Inventory ====//
	void CacheInventoryComponent();

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
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SetPlacementComponent(USceneComponent* NewPlacement);

	// for actually showing up the weapon or the item
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SpawnCurrentEquipment();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//void SetPlacementComponent(USceneComponent* NewPlacement);// temporally put on public for testing

private:
	//== Mouse scroll action
	void OnScrollChunkStart(float ScrollDirection);
	void OnScrollChunkStep(float ScrollDirection);
	void OnScrollChunkEnd(float ScrollDirection);
	void ProcessScrollDetection(float ScrollDltaValue, float DeltaTime);


	// Equipment setting
	void SpawnEquipmentInSlot(int32 ID,  EEquipmentType Type,TMap<int32, TObjectPtr<AActor>>& Slot);

public:

	// fill quick slots with equipments
	void UpdateQuickSlots();

	void UpdateWeaponQuickSlots();
	void UpdateItemQuickSlots();

	//Temp for controlling weapon without inventory( weapon in the editor )
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void TestEquipWeapon(AActor* SettingWeapon);

	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void UpdatePlacementComponent(USceneComponent* NewPlacement);// this will update where to be attatched
	void SetCurrentEquipmentPlacement();// this will attatch the current weapon or item to the placementcomponent
	UFUNCTION()
	void SwtichWeapon_PC(const FInputActionValue& Value);// will be done by mouse wheel scroll(one scroll== one switch)
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