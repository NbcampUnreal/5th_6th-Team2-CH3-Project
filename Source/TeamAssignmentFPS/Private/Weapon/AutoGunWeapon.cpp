// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/AutoGunWeapon.h"


// Sets default values
AAutoGunWeapon::AAutoGunWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnInterval = 0.15f; // fire rate for auto-fire
	MaxAmmoCount = 30;
	CurrentAmmoCount = MaxAmmoCount;
	ReloadTime = 2.f;

	// Burst settings
	MaxBurstShots = 3;
	BurstShotsFired = 0;
	bIsAutoFiring = false;
}

void AAutoGunWeapon::AutoFire()
{
	if (!bIsAutoFiring || CurrentAmmoCount <= 0)
	{
		bIsAutoFiring = false;
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
		return;
	}

	FireWeapon();
	++BurstShotsFired;

	// Stop burst after max shots for tap
	if (bIsTapBurst && BurstShotsFired >= MaxBurstShots)
	{
		bIsAutoFiring = false;
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
	}
}

void AAutoGunWeapon::OnInputTap_Implementation()
{
	if (bIsReloading || CurrentAmmoCount <= 0) return;

	bIsTapBurst = true;
	bIsAutoFiring = true;
	BurstShotsFired = 0;

	GetWorldTimerManager().SetTimer(
		AutoFireTimerHandle,
		this,
		&AAutoGunWeapon::AutoFire,
		SpawnInterval,
		true
	);
}

void AAutoGunWeapon::OnInputHoldStart_Implementation()
{
	if (bIsReloading || CurrentAmmoCount <= 0) return;

	bIsTapBurst = false; // hold mode
	bIsAutoFiring = true;

	GetWorldTimerManager().SetTimer(
		AutoFireTimerHandle,
		this,
		&AAutoGunWeapon::AutoFire,
		SpawnInterval,
		true
	);
}

void AAutoGunWeapon::OnInputRelease_Implementation()
{
	bIsAutoFiring = false;
	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}
