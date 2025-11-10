// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PistolWeapon.h"


// Sets default values
APistolWeapon::APistolWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Default settings for pistol
	MaxAmmoCount = 12;
	CurrentAmmoCount = MaxAmmoCount;
	SpawnInterval = 0.2f; 
	ReloadTime = 1.5f;
}

void APistolWeapon::AutoFire()
{
	if (bIsReloading || CurrentAmmoCount <= 0)
	{
		// stop auto-fire if no ammo or reloading
		OnInputRelease_Implementation();

		if (CurrentAmmoCount <= 0)
		{
			ReloadWeapon();
		}
		return;
	}

	FireWeapon();
}

void APistolWeapon::OnInputTap_Implementation()
{
	FireWeapon();
}

void APistolWeapon::OnInputHoldStart_Implementation()
{
	if (!bIsAutoFiring)
	{
		FireWeapon(); // first shot immediately
		bIsAutoFiring = true;
		GetWorldTimerManager().SetTimer(
			AutoFireTimerHandle,
			this,
			&APistolWeapon::AutoFire,
			SpawnInterval,
			true
		);
	}
}

void APistolWeapon::OnInputRelease_Implementation()
{
	if (bIsAutoFiring)
	{
		bIsAutoFiring = false;
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
	}
}

