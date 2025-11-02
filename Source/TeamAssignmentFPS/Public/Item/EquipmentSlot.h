// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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




// slot struct to manage equipments
USTRUCT(BlueprintType)
struct FEquipmentQuickSlots//multiple slots
{
	GENERATED_BODY()


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Equipment Slot")
	TMap<uint8/*slot Number*/,TObjectPtr<AActor>/*Equipment*/> EquipmentQuickSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Equipment Slot")
	int32 SlotMaxCount;
	

private:
	
	int32 CurrentEquipmentID=INDEX_NONE;//default
	int32 UsedSlotCount=0;
	uint8 CurrentSlotIndex=1;// the main
	
	UPROPERTY(VisibleAnywhere, Category = "Equipment Slot")
	AActor* CurrentSlotEquipment=nullptr;// not currently equipt by player but equipment of current slot index
	
public:
	AActor* GetEquipmentFromCurrentSlot() const { return CurrentSlotEquipment;}
	uint8 GetCurrentSlotIndex() const { return CurrentSlotIndex; }
	int32 GetCurrentEquipmentID() const { return CurrentEquipmentID;}

	bool AddToSlot(uint8 SlotIndex, TObjectPtr<AActor> Equipment);
	bool RemoveFromSlot(uint8 SlotIndex);

};