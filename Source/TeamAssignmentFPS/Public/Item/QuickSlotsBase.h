// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Item/EquipmentSlot.h"
#include "Debug/UELOGCategories.h"


#include "QuickSlotsBase.generated.h"

// slot struct to manage equipments
UCLASS()
class TEAMASSIGNMENTFPS_API UEquipmentQuickSlots:public UObject//multiple slots
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment Slot")
	TMap<uint8,TObjectPtr<UEquipmentSlot>> EquipmentQuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment Slot")
	int32 SlotMaxCount;
	
	int32 CurrentEquipmentID=INDEX_NONE;//default
	int32 UsedSlotCount=0;
	uint8 CurrentSlotIndex=1;// the main
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot")
	AActor* CurrentSlotEquipment=nullptr;// not currently equipt by player but equipment of current slot index

	void OnEquipmentSlotHasNothing(uint8 RemovingSlotIndex);
	
public:
	AActor* GetEquipmentFromCurrentSlot() const { return CurrentSlotEquipment;}
	uint8 GetCurrentSlotIndex() const { return CurrentSlotIndex; }
	int32 GetCurrentEquipmentID() const { return CurrentEquipmentID;}

	bool FindAvailableSlot(uint8& SlotIndex);
	

	template<typename T_SlotData>
	bool AddToQuickSlot(T_SlotData SlotData)
	{
		uint8 SlotIndex;
		if (!FindAvailableSlot(SlotIndex))
		{
			return false;
		}
		// found the slot to add
		
		return AddToSlotBySlotIndex(SlotIndex,SlotData);
	}

	template<typename T_SlotData>
	bool AddToSlotBySlotIndex(uint8 SlotIndex, T_SlotData SlotData)
	{
		if (EquipmentQuickSlot.Contains(SlotIndex))
		{
			UE_LOG(Equipment_Manager_Log, Error, TEXT("FEquipmentQuickSlots::AddToSlot-> %d is used index"),SlotIndex);
			return false;
		}

		if (UsedSlotCount >= SlotMaxCount)
		{
			UE_LOG(Equipment_Manager_Log, Error, TEXT("FEquipmentQuickSlots::AddToSlot-> Slots Occupied"));
			return false;
		}

		EquipmentQuickSlot.Add(SlotIndex, SlotData);
		++UsedSlotCount;
		UE_LOG(Equipment_Manager_Log, Log, TEXT("FEquipmentQuickSlots::AddToSlot-> Slot Added"));
		return true;
	}
	
	bool RemoveFromSlot(uint8 SlotIndex);

};