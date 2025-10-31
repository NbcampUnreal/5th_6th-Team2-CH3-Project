// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryManagerComponent.h"
#include "Debug/UELOGCategories.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UInventoryManagerComponent::GetWeaponFromInventoryByIndex(uint8 Index)
{
	AActor* FoundWeapon=nullptr;
	FWeaponData* FoundWeaponPtr=Weapons.Find(Index);
	
	if (!FoundWeaponPtr)
	{
		UE_LOG(LogTemp, Error, TEXT("UInventoryManagerComponent::GetWeaponFromInventoryByIndex-> Weapon not found"));
		return FoundWeapon;
	}

	FName FoundWeaponName=FoundWeaponPtr->WeaponName;
	UE_LOG(LogTemp, Log,
		TEXT("UInventoryManagerComponent::GetWeaponFromInventoryByIndex-> %s is Found"),*FoundWeaponName.ToString());

	//return FoundWeaponPtr->WeaponClass;//--> how should i return a weapon? should it be static class or instance, but not spawned yet?
	return FoundWeapon;// Not done yet
}

