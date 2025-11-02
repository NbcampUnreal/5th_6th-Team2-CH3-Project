// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/QuickSlotsBase.h"
#include "ChildClassesOfQuickSlots.generated.h"

/*
 * use template in here!!!! make child class of equipment quick slots 
 */
UCLASS()
class TEAMASSIGNMENTFPS_API UWeaponQuickSlots : public UEquipmentQuickSlots
{
	GENERATED_BODY()
public:
	bool AddWeaponSlot(UWeaponSlot* WeaponSlot)
	{
		return AddToQuickSlot<UWeaponSlot>(WeaponSlot);
	}
	
	UWeaponSlot* GetWeaponSlot(uint8 SlotIndex)
	{
		return GetSlot<UWeaponSlot>(SlotIndex);
	}
	
	bool InitializeWeaponSlot(int32 EquipmentID,const TSubclassOf<AActor>& EquipmentClass)
	{
		FInitializeParams Params;
		Params.ID = EquipmentID;

		return InitializeSlot<UWeaponSlot>(Params, EquipmentClass);
	}

};

UCLASS()
class TEAMASSIGNMENTFPS_API UItemQuickSlots : public UEquipmentQuickSlots
{
	GENERATED_BODY()
public:
	bool AddWeaponSlot(UWeaponSlot* WeaponSlot)
	{
		return AddToQuickSlot<UWeaponSlot>(WeaponSlot);
	}
	
	UWeaponSlot* GetWeaponSlot(uint8 SlotIndex)
	{
		return GetSlot<UWeaponSlot>(SlotIndex);
	}
	
	bool InitializeItemSlot(int32 EquipmentID,const TSubclassOf<AActor>& EquipmentClass, int32 MaxStack)
	{
		FInitializeParams Params;
		Params.ID = EquipmentID;
		Params.MaxStack = MaxStack;

		return InitializeSlot<UItemSlot>(Params, EquipmentClass);
	}
};


