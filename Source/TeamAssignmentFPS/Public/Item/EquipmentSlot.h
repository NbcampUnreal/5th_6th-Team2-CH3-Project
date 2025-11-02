// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"// to use delegate, struct cannot boradcast or execute delegate

UCLASS()
class TEAMASSIGNMENTFPS_API UEquipmentSlot: public UObject
{
	GENERATED_BODY()
public:
	/*DECLARE_MULTICAST_DELEGATE(FOnEquipmentChanged);
	FOnEquipmentChanged OnEquipmentChanged;*/ //temp delegate. find the required params 
	
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

UCLASS()
class TEAMASSIGNMENTFPS_API UWeaponSlot:public UEquipmentSlot// one slot
{
	GENERATED_BODY()

	/*
	 TODO: put required infos in here for weapon

	 */
	
protected:
	bool InitializeWeaponData(AActor* Equipment,int32 ID);
};

UCLASS()
class TEAMASSIGNMENTFPS_API UItemSlot:public UEquipmentSlot// one slot
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