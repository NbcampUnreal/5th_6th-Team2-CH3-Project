// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileWeaponBase.h"
#include "AutoGunWeapon.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API AAutoGunWeapon : public AProjectileWeaponBase
{
	GENERATED_BODY()

public:
	AAutoGunWeapon();

protected:
	// Auto-fire timer
	FTimerHandle AutoFireTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon | AutoGun")
	int32 MaxBurstShots = 3;
	
	int32 BurstShotsFired = 0;// counting

	bool bIsTapBurst=false;
	bool bIsAutoFiring=false;

	// Called repeatedly while holding
	void AutoFire();

public:
	// Input overrides
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputRelease_Implementation() override;
};
