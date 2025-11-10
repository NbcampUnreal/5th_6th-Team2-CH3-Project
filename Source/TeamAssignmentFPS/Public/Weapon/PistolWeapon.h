// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileWeaponBase.h"
#include "PistolWeapon.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API APistolWeapon : public AProjectileWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APistolWeapon();

protected:
	
	FTimerHandle AutoFireTimerHandle;
	bool bIsAutoFiring;
	void AutoFire();
	
public:
	
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputRelease_Implementation() override;
};
