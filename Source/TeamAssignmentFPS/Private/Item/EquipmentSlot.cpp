// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EquipmentSlot.h"
#include "Debug/UELOGCategories.h"//debug log


// For Slots
bool UEquipmentSlot::InitializeEquipmentSlot(AActor* Equipment, int32 ID)
{
	if(!Equipment)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("FEquipmentSlot::InitializeEquipmentSlot-> Invalid EquipmentPtr"));
		return false;
	}
	EquipmentPtr=Equipment;
	EquipmentID=ID;
	return true;
}

//----- ChildClasses---------------------------------------------------------------------------------------------------//
bool UWeaponSlot::InitializeWeaponData(AActor* Equipment, int32 ID)
{
	if (!InitializeEquipmentSlot(Equipment,ID))
	{
		return false;
	}
	// now can InitializeWeaponData

	//TODO:: add something for weapondata


	//after confirmations and initializing is done
	UE_LOG(Inventory_Log, Log, TEXT("UWeaponSlot::InitializeWeaponData-> Initialization completed"));
	return true;
}


bool UItemSlot::IncrementStackCount(bool& BDidReachedMax)
{
	if (CurrentStackCount>=MaxStackCount)
	{
		BDidReachedMax=true;
		UE_LOG(Inventory_Log,Error,
			TEXT("UItemSlot::IncrementStackCount-> Not enough space. %d/%d"),CurrentStackCount,MaxStackCount);
		return false;
	}
	
	++CurrentStackCount;
	BDidReachedMax=(CurrentStackCount>=MaxStackCount);// much cleaner

	if (BDidReachedMax)
	{
		UE_LOG(Inventory_Log,Warning,
			TEXT("UItemSlot::IncrementStackCount-> Slot is now full. %d/%d"),CurrentStackCount,MaxStackCount);
	}
	return true;
}

bool UItemSlot::DecrementStackCount(bool& DidSlotConsumedAll)
{
	
	if (CurrentStackCount<=0)
	{
		UE_LOG(Inventory_Log,Error,
			TEXT("UItemSlot::DecrementStackCount-> No more Item, %d/%d"),CurrentStackCount,MaxStackCount);
		DidSlotConsumedAll=false;// not consumed at all
		return false;
	}
	
	--CurrentStackCount;//decrement
	DidSlotConsumedAll=(CurrentStackCount<=0);

	if (DidSlotConsumedAll)// after consumed, when no more item to consume
	{
		UE_LOG(Inventory_Log,Warning,
			TEXT("UItemSlot::DecrementStackCount-> No more item left, %d/%d"),CurrentStackCount,MaxStackCount);
	}
	
	return true;
}

bool UItemSlot::InitializeItemData(AActor* Equipment, int32 ID, int32 MaxStack)
{
	if (!InitializeEquipmentSlot(Equipment,ID))
	{
		return false;
	}
	// now check and initialize the item data from here

	if (MaxStack<=0)
	{
		UE_LOG(Inventory_Log,Error,
			TEXT("UItemSlot::InitializeItemData-> cannot stack item when max stack count is below 0"));
		return false;
	}
	MaxStackCount=MaxStack;
	// all done
	UE_LOG(Inventory_Log, Log, TEXT("UItemSlot::InitializeItemData-> Initialization completed"));
	return true;
}
