// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"// to use delegate, struct cannot boradcast or execute delegate
#include "EquipmentSlot.generated.h"


USTRUCT()
struct FInitializeParams
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* Equipment=nullptr;
	UPROPERTY()
	TSubclassOf<AActor> EquipmentClass = nullptr;
	UPROPERTY()
	int32 ID=INDEX_NONE;
	UPROPERTY()
	int32 MaxStack=INDEX_NONE;
	// and more
	
};




UCLASS()
class TEAMASSIGNMENTFPS_API UEquipmentSlot: public UObject
{
	GENERATED_BODY()
protected:
	/*DECLARE_MULTICAST_DELEGATE(FOnEquipmentChanged);
	FOnEquipmentChanged OnEquipmentChanged;*/ //temp delegate. find the required params 
	
	UPROPERTY()
	TObjectPtr<AActor> EquipmentPtr=nullptr;
	UPROPERTY()
	int32 EquipmentID=INDEX_NONE;

public:
	virtual bool InitializeEquipmentSlot(const FInitializeParams& Params);

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
	
public:
	virtual bool InitializeEquipmentSlot(const FInitializeParams& Params) override;
};

UCLASS()
class TEAMASSIGNMENTFPS_API UItemSlot:public UEquipmentSlot// one slot
{
	GENERATED_BODY()
	
protected:
	int32 MaxStackCount=1;// default
	int32 CurrentStackCount=0;

public:
	int32 GetCurrentStackCount()const {return CurrentStackCount;}
	int32 GetMaxStackCount()const {return MaxStackCount;}

	bool IncrementStackCount(bool& BDidReachedMax);// only increment and decrement with 1 cause the leftover case happens
	// bool param for checking if the slot is full or not after increment
	bool DecrementStackCount(bool& DidSlotConsumedAll);
	// param for checking if the slot used all of the stacks and need to removed from quick slots

	
	virtual bool InitializeEquipmentSlot(const FInitializeParams& Params) override;
};