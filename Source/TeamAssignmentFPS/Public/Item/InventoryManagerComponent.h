// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Debug/UELOGCategories.h"
#include "Item/EquipmentData.h"

#include "InventoryManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | Weapon")
	TMap<int32, FWeaponData> Weapons;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | Weapon")

	

	TArray<int32> FreedWeaponIDs;
	// this is where the removed item id is stored, so that when new id is required, it checks here first so that id gap to be filled
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | Item")
	TMap<int32, FItemData> Items;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | Item")

	TArray<int32> FreedItemIDs;// same for item ids


	//Id Setting
	int32 CurrentMaxWeaponID=0;// this is for setting 
	int32 CurrentMaxItemID=0;
	

	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	TSubclassOf<AActor> GetWeaponByID(int32 ID);
	TSubclassOf<AActor>  GetItemByID(int32 ID);


	//Inventory-> Equipment Slots
	bool AddWeapon(FWeaponData WeaponData);
	bool RemoveWeapon(int32 ID);
	
	bool AddItem(FItemData ItemData);
	bool RemoveItem(int32 ID);


	
	// ID Management
private:
	static bool GenerateID(int32& ID, TArray<int32>& FreedIDs, int32& CurrentMaxID);

	template<typename T_EquipmentData>// using template
	bool FreeID(int32 ID, TMap<int32, T_EquipmentData>& Inventory, TArray<int32>& FreedIDs)
	{
		//valid Checker
		if (!Inventory.Contains(ID))
		{
			UE_LOG(Inventory_Log, Error, TEXT("InventoryManagerComponent::FreeID -> ID %d not found in inventory!"), ID);
			return false;
		}

		//remove the id of the removed equipment//  so that when removed Equipment re-enter to the inventory, it will have new id
		T_EquipmentData& EquipmentData = Inventory[ID];
		EquipmentData.SetEquipmentID(INDEX_NONE);

		// Remove from inventory
		Inventory.Remove(ID);

		// Add to FreedIDs for recycling
		FreedIDs.Add(ID);

		UE_LOG(LogTemp, Log, TEXT("InventoryManagerComponent::FreeID -> Freed ID: %d"), ID);
		return true;
	}

// helper for finding item or weapon
	template<typename T_EquipmentData>
	bool GetEquipmentFromInventory(int32 ID, TMap<int32,T_EquipmentData>& Inventory, TSubclassOf<AActor>& EquipmentClass)
	{
		T_EquipmentData* FoundData=Inventory.Find(ID);
		if (!FoundData)
		{
			UE_LOG(LogTemp, Log,
				TEXT("InventoryManagerComponent:: GetEquipmentFromInventory -> ID %d, was not found in the given inventory"), ID);
			return false;
		}

		if (!FoundData->EquipmentClass)
		{
			UE_LOG(LogTemp, Log,
				TEXT("InventoryManagerComponent:: GetEquipmentFromInventory -> ID %d, has no valid equipment class"), ID);
			return false;
		}

		EquipmentClass = FoundData->EquipmentClass;
		return true;
	}

};
