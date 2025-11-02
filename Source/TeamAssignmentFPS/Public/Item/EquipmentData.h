// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentData.generated.h"

// this is the informations required to spawn weapons and items
// and use them in the datatable row for later

UENUM()
enum class EEquipmentType:uint8
{
	None UMETA(DisplayName = "None"),// nothing to hold
	Weapon UMETA(DisplayName="Weapon"),
	Item UMETA(DisplayName="Item"),
};



USTRUCT(BlueprintType)
struct FEquipmentData//basically a parent class of weapon and item
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data")
	EEquipmentType Type=EEquipmentType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data")
	FName DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data")
	FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data")
	TSubclassOf<AActor> EquipmentClass;

	//UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data")
	TObjectPtr<UTexture2D> IconTexture=nullptr;

private:
	int32 InventoryID=INDEX_NONE;
public:
	void SetEquipmentID(int32 ID){InventoryID=ID;}
	void GetEquipmentID(int32& ID)const {ID=InventoryID;}
};


USTRUCT(BlueprintType)
struct FWeaponData:public FEquipmentData//child of equipment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data | Weapon")
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data | Weapon")
	float MaxAmoCount;

	// data for weapons

};

USTRUCT(BlueprintType)
struct FItemData:public FEquipmentData//child of equipment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data | Item")
	int32 SlotStackMaxCount;// max amount of stack count per slot

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data | Item")
	bool bIsConsumable;// does it disappear after using item or permanent?

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category="Equipment Data | Item")
	float CoolDownDuration;

	// data for weapons
	//maybe price? (sell price, multiplyer for sell and buy?)

};
