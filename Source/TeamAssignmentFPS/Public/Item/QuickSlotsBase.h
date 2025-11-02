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
	int32 SlotMaxCount=5;//default
	
	int32 CurrentEquipmentID=INDEX_NONE;//default
	int32 UsedSlotCount=0;
	uint8 CurrentSlotIndex=1;// the main
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot")
	AActor* CurrentSlotEquipment=nullptr;// not currently equipt by player but equipment of current slot index


	
public:
	AActor*GetCurrentEquipmentPtr() const {return CurrentSlotEquipment;}
	int32 GetCurrentEquipmentID() const {return CurrentEquipmentID;}
	uint8 GetCurrentSlotIndex() const {return CurrentSlotIndex;}

	bool SwitchCurrentSlot(uint8 SlotIndex);// set the current slot index and get the current values after that
	
	bool FindAvailableSlot(uint8& SlotIndex) const;// auto slot switch
	bool RemoveFromQuickSlot(uint8 SlotIndex);

	
	
//Template Helper function
protected:
	template<typename T_SlotData>
	bool AddToQuickSlot(T_SlotData SlotData)
	{
		if (!SlotData )
		{
			UE_LOG(Equipment_Manager_Log, Error, TEXT("UEquipmentQuickSlots::AddToQuickSlot-> Invalid SlotData"));
			return false;
		}
		uint8 SlotIndex;
		if (!FindAvailableSlot(SlotIndex))
		{
			return false;
		}
		// found the slot to add
		
		return AddToSlotBySlotIndex(SlotIndex,SlotData);
	}
	template<typename T_SlotData>
	bool AddToQuickSlotBySlotIndex(uint8 SlotIndex, T_SlotData SlotData)
	{
		if (EquipmentQuickSlot.Contains(SlotIndex))
		{
			UE_LOG(Equipment_Manager_Log, Error, TEXT("UEquipmentQuickSlots::AddToSlot-> %d is used index"),SlotIndex);
			return false;
		}

		if (UsedSlotCount >= SlotMaxCount)
		{
			UE_LOG(Equipment_Manager_Log, Error, TEXT("UEquipmentQuickSlots::AddToSlot-> Slots Occupied"));
			return false;
		}

		EquipmentQuickSlot.Add(SlotIndex, SlotData);
		++UsedSlotCount;
		UE_LOG(Equipment_Manager_Log, Log, TEXT("UEquipmentQuickSlots::AddToSlot-> Slot Added"));
		return true;
	}
	template<typename T_SlotClass>
	T_SlotClass* GetSlot(uint8 SlotIndex)
	{
		if (!EquipmentQuickSlot.Contains(SlotIndex))
		{
			UE_LOG(Equipment_Manager_Log,Error,
				TEXT("UEquipmentQuickSlots::GetSlot-> Index %d cannot be found"),SlotIndex);
			return nullptr;
		}
		return Cast<T_SlotClass>(EquipmentQuickSlot[SlotIndex]);
	}
// internally called functions
	void OnEquipmentSlotHasNothing(uint8 RemovingSlotIndex);
	bool CheckIfIndexIsValid(uint8 SlotIndex);
};