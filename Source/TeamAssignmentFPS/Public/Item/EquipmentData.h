// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.generated.h"

// this is the informations required to spawn weapons and items
// and use them in the datatable row for later

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category="Weapon Data")
	FName WeaponName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly ,Category="Weapon Data")
	FString Description;
	UPROPERTY()
	TSubclassOf<AActor> WeaponClass;
	// etc


private:
	uint32 WeaponID=0;

public:
	void SetWeaponID(uint32 ID) {WeaponID=ID;}
	uint32 GetWeaponID()const {return WeaponID;}
};

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()
	
	
};