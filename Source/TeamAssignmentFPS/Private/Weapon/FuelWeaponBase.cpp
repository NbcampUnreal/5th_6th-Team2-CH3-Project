// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FuelWeaponBase.h"


// Sets default values
AFuelWeaponBase::AFuelWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFuelWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFuelWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AFuelWeaponBase::OnReloadInputPressed_Implementation()
{
	IWeaponInterface::OnReloadInputPressed_Implementation();
}

void AFuelWeaponBase::OnInputHoldStart_Implementation()
{
	IInputReactionInterface::OnInputHoldStart_Implementation();
}

void AFuelWeaponBase::OnInputHoldUpdate_Implementation(float InputValue)
{
	IInputReactionInterface::OnInputHoldUpdate_Implementation(InputValue);
}

void AFuelWeaponBase::OnInputRelease_Implementation()
{
	IInputReactionInterface::OnInputRelease_Implementation();
}

void AFuelWeaponBase::OnEquipped_Implementation()
{
	IEquipmentInterface::OnEquipped_Implementation();
}

void AFuelWeaponBase::OnUnequipped_Implementation()
{
	IEquipmentInterface::OnUnequipped_Implementation();
}

// Called every frame
void AFuelWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFuelWeaponBase::FireWeapon()
{
}

void AFuelWeaponBase::StopFiringWeapon()
{
}

