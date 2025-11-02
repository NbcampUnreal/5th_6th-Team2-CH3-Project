// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Debug/UELOGCategories.h"

USTRUCT(BlueprintType)
struct FEquipmentSlot// one slot
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<AActor> EquipmentPtr=nullptr;

private:
	UPROPERTY()
	int32 EquipmentID=INDEX_NONE;

public:
	bool InitializeEquipmentSlot(AActor* Equipment,int32 ID);

	int32 GetEquipmentID()const {return EquipmentID;}
	AActor* GetEquipmentPtr() const {return EquipmentPtr;}
};

// make Child class of slots (Weapon, Items)

USTRUCT(BlueprintType)
struct FWeaponSlot:public FEquipmentSlot// one slot
{
	GENERATED_BODY()

	/*
	 TODO: put required infos in here for weapon

	 */
	
protected:
	bool InitializeWeaponData(AActor* Equipment,int32 ID);
};

USTRUCT(BlueprintType)
struct FItemSlot:public FEquipmentSlot// one slot
{
	GENERATED_BODY()
	
private:
	int32 MaxStackCount=0;
	int32 CurrentStackCount=INDEX_NONE;

public:
	int32 GetCurrentStackCount()const {return MaxStackCount;}
	int32 GetMaxStackCount()const {return CurrentStackCount;}

	bool IncrementStackCount(bool& BDidReachedMax);// only increment and decrement with 1 cause the leftover case happens
	// bool param for checking if the slot is full or not after increment
	bool DecrementStackCount(bool& DidSlotConsumedAll);
	// param for checking if the slot used all of the stacks and need to removed from quick slots

	
	bool InitializeItemData(AActor* Equipment,int32 ID, int32 MaxStack);
};