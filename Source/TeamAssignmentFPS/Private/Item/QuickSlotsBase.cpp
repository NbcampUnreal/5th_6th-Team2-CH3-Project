// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/QuickSlotsBase.h"// for EquipmentQuickSlot


bool UEquipmentQuickSlots::RemoveFromSlot(uint8 SlotIndex)
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
	//TODO: set the slot to slot[0], the default main one
	return false;// temp
}