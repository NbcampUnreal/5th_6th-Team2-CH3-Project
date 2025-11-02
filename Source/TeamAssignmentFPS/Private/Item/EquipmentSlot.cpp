// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EquipmentSlot.h"
#include "Debug/UELOGCategories.h"//debug log


// For Slots
bool FEquipmentSlot::InitializeEquipmentSlot(AActor* Equipment, int32 ID)
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
bool FWeaponSlot::InitializeWeaponData(AActor* Equipment, int32 ID)
{
	if (!InitializeEquipmentSlot(Equipment,ID))
	{
		return false;
	}
	// now can InitializeWeaponData

	//TODO:: add something for weapondata


	//after confirmations and initializing is done
	return true;
}

bool FItemSlot::IncrementStackCount(bool& BDidReachedMax)
{
	if (CurrentStackCount>=MaxStackCount)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("FItemSlot::IncrementStackCount-> Not enough space. %d/%d"),CurrentStackCount,MaxStackCount);
		BDidReachedMax=false;
		return false;
	}
	++CurrentStackCount;

	if (CurrentStackCount>=MaxStackCount)
	{
		BDidReachedMax=true;
		UE_LOG(Equipment_Manager_Log,Warning,
			TEXT("FItemSlot::IncrementStackCount-> Slot is now full. %d/%d"),CurrentStackCount,MaxStackCount);
	}
	
	BDidReachedMax=false;
	return true;
}

bool FItemSlot::DecrementStackCount(bool& DidSlotConsumedAll)
{
	
	if (CurrentStackCount<=0)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("FItemSlot::DecrementStackCount-> No more Item, %d/%d"),CurrentStackCount,MaxStackCount);
		DidSlotConsumedAll=false;// not consumed at all
		return false;
	}
	
	--CurrentStackCount;//decrement

	if (CurrentStackCount<=0)// after consumed, when no more item to consume
	{
		DidSlotConsumedAll=true;
		UE_LOG(Equipment_Manager_Log,Warning,
			TEXT("FItemSlot::DecrementStackCount-> No more item left, %d/%d"),CurrentStackCount,MaxStackCount);
	}
	
	DidSlotConsumedAll=false;
	return true;
}

bool FItemSlot::InitializeItemData(AActor* Equipment, int32 ID, int32 MaxStack)
{
	if (!InitializeEquipmentSlot(Equipment,ID))
	{
		return false;
	}
	// now check and initialize the item data from here

	if (MaxStack<=0)
	{
		UE_LOG(Equipment_Manager_Log,Error,
			TEXT("FItemSlot::InitializeItemData-> cannot stack item when max stack count is below 0"));
		return false;
	}
	MaxStackCount=MaxStack;

	return true;
}

// for EquipmentQuickSlot
bool FEquipmentQuickSlots::AddToSlot(uint8 SlotIndex, TObjectPtr<AActor> Equipment)
{
	if(!Equipment)
	{
		UE_LOG(Equipment_Manager_Log, Error, 
		TEXT("FEquipmentQuickSlot::AddToSlot-> Invalid Equipment"));
		return false;
	}
	if (EquipmentQuickSlot.Contains(SlotIndex))
	{
		UE_LOG(Equipment_Manager_Log, Error, 
		TEXT("FEquipmentQuickSlot::AddToSlot-> Slot.%d is already used"),SlotIndex);
		return false;
	}
	if (UsedSlotCount>=SlotMaxCount)// no place to add
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("FEquipmentQuickSlot::AddToSlot-> Not enough slot to put"));
		return false;
	}
	// checking completed
	
	EquipmentQuickSlot.Add(SlotIndex, Equipment);

	if (UsedSlotCount==0)
	{
		CurrentSlotIndex=SlotIndex;
		CurrentSlotEquipment=Equipment;
	}
	++UsedSlotCount;
	return true;

	//Should it set the currentEquipment to newly added one?
	// ex. Gain weapon-> auto switch to new weapon
	// or Gain Weapon-> stays in current weapon -> manually switch to new weapon if player request
}

bool FEquipmentQuickSlots::RemoveFromSlot(uint8 SlotIndex)
{
	if (!EquipmentQuickSlot.Contains(SlotIndex))
	{
		UE_LOG(Equipment_Manager_Log, Error, TEXT("FEquipmentQuickSlot::RemoveFromSlot-> ID:%d not found in the slot"),SlotIndex);
		return false;
	}

	EquipmentQuickSlot.Remove(SlotIndex);
	--UsedSlotCount;

	if (CurrentEquipmentID!=SlotIndex)// when removed equipment was not from current slot
	{
		return true;// just return
	}

	// removed one is current slot. find new slot to replace current slot

	if (UsedSlotCount<=0)// empty slots. no equipments
	{
		CurrentSlotIndex=INDEX_NONE;
		CurrentSlotEquipment=nullptr;
		return true;
		// should it set to the first one? or last one?
	}
	// when there is a replacement slot

	return false;// temp
}