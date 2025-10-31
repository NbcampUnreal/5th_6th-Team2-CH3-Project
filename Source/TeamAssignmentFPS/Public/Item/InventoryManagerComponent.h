// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/EquipmentData.h"

#include "InventoryManagerComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | Weapon")
	TMap<FName, FWeaponData> Weapons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory | Item")
	TMap<FName, FItemData> Items;
	
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	AActor* GetWeaponOrItemFromDataRow();
		
};
