// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryManagerComponent.h"
#include "Debug/UELOGCategories.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

TSubclassOf<AActor> UInventoryManagerComponent::GetWeaponByID(int32 ID)
{
	TSubclassOf<AActor> WeaponClass=nullptr;
	if (!GetEquipmentFromInventory<FWeaponData>(ID, Weapons, WeaponClass))
	{
		// what should this do????.... fuck
		//--> should send signal to ui?
	}
	
	return WeaponClass;
}

TSubclassOf<AActor> UInventoryManagerComponent::GetItemByID(int32 ID)
{
	TSubclassOf<AActor> ItemClass=nullptr;
	if (!GetEquipmentFromInventory<FItemData>(ID, Items, ItemClass))
	{
		// what should this do????.... fuck
	}
	
	return ItemClass;
}

bool UInventoryManagerComponent::AddWeapon(FWeaponData WeaponData)
{
	int32 WeaponID;

	if (!GenerateID(WeaponID,FreedWeaponIDs,CurrentMaxWeaponID))
	{
		//UE_LOG(Inventory_Log,Error, TEXT("UInventoryManagerComponent::AddWeapon-> "))
		////--> this will be done in the generate, so no need
		return false;
	}
	
	WeaponData.SetEquipmentID(WeaponID);
	Weapons.Add(WeaponID,WeaponData);
	return true;
}

bool UInventoryManagerComponent::RemoveWeapon(int32 ID)
{
	return FreeID<FWeaponData>(ID, Weapons, FreedWeaponIDs);
}

bool UInventoryManagerComponent::AddItem(FItemData ItemData)
{
	int32 ItemID;
	if (!GenerateID(ItemID,FreedItemIDs,CurrentMaxItemID))
	{
		return false;
	}

	ItemData.SetEquipmentID(ItemID);
	Items.Add(ItemID,ItemData);
	return true;
}

bool UInventoryManagerComponent::RemoveItem(int32 ID)
{
	return FreeID<FItemData>(ID, Items, FreedItemIDs);
}

bool UInventoryManagerComponent::GetWeaponDataByID(int32 ID, FWeaponData& OutData) const
{
	if (const FWeaponData* Found = Weapons.Find(ID))
	{
		OutData = *Found;
		return true;
	}

	UE_LOG(Inventory_Log, Warning, TEXT("UInventoryManagerComponent::GetWeaponDataByID -> ID %d not found."), ID);
	return false;
}

bool UInventoryManagerComponent::GetItemDataByID(int32 ID, FItemData& OutData) const
{
	if (const FItemData* Found = Items.Find(ID))
	{
		OutData = *Found;
		return true;
	}

	UE_LOG(Inventory_Log, Warning, TEXT("UInventoryManagerComponent::GetItemDataByID -> ID %d not found."), ID);
	return false;
}

bool UInventoryManagerComponent::GenerateID(int32& ID, TArray<int32>& FreedIDs, int32& CurrentMaxID)
{
	//Valid Checking first
	// TODO: valid check for params
	if (CurrentMaxID<0)
	{
		UE_LOG(Inventory_Log,Error, TEXT("InventoryManagerComponent::GenerateID -> MaxID is Negative."))
		return false;
	}
	// are there other things to check?

	// --> checking completed

	// Case1: When there is usable freed id in the list
	if (!FreedIDs.IsEmpty())
	{
		/*ID=FreedIDs[0];
		FreedIDs.RemoveAt(0);*/ // for faster, use pop!!!!

		ID=FreedIDs[FreedIDs.Num()-1];
		FreedIDs.Pop();// like this!
			
		UE_LOG(LogTemp,Log,TEXT("InventoryManagerComponent::GenerateID -> Recycled NewID:%d"),ID);
		return true;
	}
	// Case2: when there is no freed id to use, so make new one
	ID=++CurrentMaxID;

	UE_LOG(LogTemp,Log,TEXT("InventoryManagerComponent::GenerateID -> NewID:%d"),ID);
	return true;
}


