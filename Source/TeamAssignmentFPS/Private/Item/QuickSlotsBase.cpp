// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/QuickSlotsBase.h"// for EquipmentQuickSlot


void UEquipmentQuickSlots::OnEquipmentSlotHasNothing(uint8 RemovingSlotIndex)
{
	if (CurrentSlotIndex == RemovingSlotIndex)
	{
		CurrentSlotEquipment = nullptr;
		CurrentEquipmentID = INDEX_NONE;
		UE_LOG(Equipment_Manager_Log, Log,
			TEXT("UEquipmentQuickSlots::OnEquipmentSlotHasNothing->Slot %d emptied, current equipment reset."),
			RemovingSlotIndex);
	}
}


bool UEquipmentQuickSlots::SwitchCurrentSlot(uint8 SlotIndex)
{
	if (!CheckIfIndexIsValid(SlotIndex))
	{
		return false;
	}

	CurrentSlotEquipment=EquipmentQuickSlot[SlotIndex]->GetEquipmentPtr();
	CurrentEquipmentID= EquipmentQuickSlot[SlotIndex]->GetEquipmentID();
	CurrentSlotIndex=SlotIndex;
	return true;
}

bool UEquipmentQuickSlots::SwitchToNextSlot(bool bIsDirectionRight)
{
	if (EquipmentQuickSlot.IsEmpty()||EquipmentQuickSlot.Num()==1)
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentQuickSlots::SwitchToNextSlot-> No Slot to Switch"));
		return false;
	}

	uint8 NewIndex=CurrentSlotIndex;
	do// wow, first time using this. fuck yeah
	{
		if (bIsDirectionRight)
		{
			++NewIndex;
			if (NewIndex>SlotMaxCount)//when it go beyond max, start over
			{
				NewIndex=1;
			}
		}
		else
		{
			--NewIndex;
			if (NewIndex<1)// when it goes beyond the min, go to max count
			{
				NewIndex=SlotMaxCount;
			}
		}
	}	while (!EquipmentQuickSlot.Contains(NewIndex));
	
	// scrolling done
	
	return SwitchCurrentSlot(NewIndex);
}

bool UEquipmentQuickSlots::FindAvailableSlot(uint8& SlotIndex) const
{
	for (uint8 i = 1; i <= SlotMaxCount; ++i)//simple loop
	{
		if (!EquipmentQuickSlot.Contains(i))
		{
			SlotIndex = i;
			return true;
		}
	}
	return false;
}

bool UEquipmentQuickSlots::RemoveFromQuickSlot(uint8 SlotIndex)
{
	if (!EquipmentQuickSlot.Contains(SlotIndex))
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentQuickSlots::RemoveFromSlot-> ID:%d not found in the slot"),SlotIndex);
		return false;
	}

	EquipmentQuickSlot.Remove(SlotIndex);
	--UsedSlotCount;
	
	if (CurrentEquipmentID!=SlotIndex)// when removed equipment was not from the current slot
	{
		return true;// just return
	}

	// removed one is current slot. find new slot to replace current slot

	if (UsedSlotCount<=0)// empty slots. no equipments
	{
		OnEquipmentSlotHasNothing(SlotIndex);
		return true;
		// should it set to the first one? or the last one?
	}
	// when there is a replacement slot

	if (EquipmentQuickSlot.Contains(1))// if the leftover slots have the default main weapon slot
	{
		SwitchCurrentSlot(1);
		UE_LOG(Equipment_Manager_Log, Log,
			TEXT("Current slot switched to %d after removal"), CurrentSlotIndex);
		return true;
	}

	uint8 NewSlotIndex=EquipmentQuickSlot.CreateIterator().Key();// for finding another available index from TMap
	SwitchCurrentSlot(NewSlotIndex);
	UE_LOG(Equipment_Manager_Log, Log,
		TEXT("Current slot switched to %d after removal"), CurrentSlotIndex);
	
	return true;
}



bool UEquipmentQuickSlots::CheckIfIndexIsValid(uint8 SlotIndex)
{
	if (!EquipmentQuickSlot.Find(SlotIndex))
	{
		UE_LOG(Equipment_Manager_Log, Error,
			TEXT("UEquipmentQuickSlots::CheckIfIndexIsValid->Index %d isn't there"),SlotIndex);
		return false;
	}
	return true;
}
