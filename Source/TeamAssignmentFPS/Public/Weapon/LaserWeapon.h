// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FuelWeaponBase.h"
#include "LaserWeapon.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API ALaserWeapon : public AFuelWeaponBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaserWeapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Laser")
	float MaxDistance = 5000.0f; // 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Laser | Visual")
	FColor LaserColor = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Laser | Visual")
	float LaserThickness = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Laser | Damage")
	bool bDealDamage = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Laser | Damage")
	float DamagePerSecond = 10.0f;

	// VFX
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Laser | VFX")
	TObjectPtr<UNiagaraSystem> LaserVFX;// laser line

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile | VFX")
	TObjectPtr<UParticleSystem> LaserHitImpactVFX;//Impact VFX*/

protected:
	virtual void Tick(float DeltaSeconds) override;
	virtual void FireWeapon() override;
	virtual void StopFiringWeapon() override;

private:
	void PerformLaserTrace(float DeltaSeconds);

	void GiveDamageToActor(AActor* HitActor);
};