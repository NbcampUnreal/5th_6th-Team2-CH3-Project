// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/QuickSlotsBase.h"
#include "ChildClassesOfQuickSlots.generated.h"

/*
 * use template in here!!!! make child class of equipment quick slots 
 */
UCLASS()
class TEAMASSIGNMENTFPS_API UWEaponQuickSlots : public UEquipmentQuickSlots
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
};

