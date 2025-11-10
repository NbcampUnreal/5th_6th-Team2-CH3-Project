// ProjectileWeaponParabola.h
#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileWeaponBase.h"
#include "Parabola_ProjectileBase.h"
#include "ParabolaWeapon.generated.h"

class ULockonComponent;

UCLASS()
class TEAMASSIGNMENTFPS_API AParabolaWeapon : public AProjectileWeaponBase
{
	GENERATED_BODY()

public:
	AParabolaWeapon();

protected:

	UPROPERTY()
	ULockonComponent* LockonComponent=nullptr;// to get the landing location
	
	/** Charging and input */
	bool bIsCharging = false;
	bool bIsCharged = false;
	float CurrentChargeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Charging")
	float MaxChargeTime = 2.f;

	/** Parabola settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MinThrowDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MinParabolaHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MaxParabolaHeight = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	int32 PathSegments = 24;

	//Temp path indicator
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	FColor PathColor_Uncharged = FColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	FColor PathColor_Charged = FColor::Green;


	//debug draw custom tick
	FTimerHandle ParabolaDrawTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
    float ParabolaDrawInterval = 0.03f; 
    
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	
	/** InputReactionInterface */
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;

	void FireParabolaProjectile();
	/** Fire the parabola projectile */
	void LaunchParabolaProjectile();

	void TossParabolaProjectile();

	// Debug checking
	UFUNCTION(BlueprintPure)
	bool IsCharging() const {return bIsCharging;}

	/** Draw debug path */
	void DrawParabolaPath(float ChargeRatio);

	void DrawParabolaTimerTick();

	//equpiment
	virtual void OnEquipped_Implementation() override;
};
