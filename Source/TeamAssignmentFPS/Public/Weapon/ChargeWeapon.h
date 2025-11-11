// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileWeaponBase.h"
#include "Curves/CurveFloat.h"// for more dynamic chargning scale value
#include "ChargeWeapon.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API AChargeWeapon : public AProjectileWeaponBase
{
	GENERATED_BODY()

public:
	AChargeWeapon();

protected:
	// === Charging === //
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	float MinChargeTime = 0.2f;// min requirement 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	float MaxChargeTime = 1.5f;// max requirement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	float CurrentChargeTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Charging")
	float ChargeRate = 1.0f;// charge speed
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	bool bIsCharging = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	bool bIsChargingStopped = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Charging")
	TObjectPtr<UCurveFloat> ChargeCurve = nullptr;

	// For visual or sound feedback while charging
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | FX")
	TObjectPtr<UParticleSystem> ChargingEffect= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | FX")
	TObjectPtr<USoundBase> ChargingSound= nullptr;

	FTimerHandle ChargingTimerHandle;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	virtual void StartCharging();
	
	virtual void UpdateCharging(float DeltaTime);

	virtual void StopCharging();
	
	virtual void ReleaseCharge();// could be used for decrementing scalar value or just simple reset

	virtual void ResetCharge();// this is for instant reset

protected:
	UFUNCTION(BlueprintCallable, Category = "Weapon | Charging")
	float GetChargeRatio() const;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon | Charging")
	virtual void FireChargedProjectile(float ChargeRatio);
	


	UFUNCTION(BlueprintCallable, Category = "Weapon | Charging")
	virtual bool IsMinChargeSatisfied() const;
};
