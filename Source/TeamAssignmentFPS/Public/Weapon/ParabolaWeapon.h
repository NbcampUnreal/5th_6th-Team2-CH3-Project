// ProjectileWeaponParabola.h
#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileWeaponBase.h"
#include "Parabola_ProjectileBase.h"
#include "ParabolaWeapon.generated.h"

<<<<<<< HEAD
<<<<<<< HEAD
class ULockonComponent;

=======
>>>>>>> 9dec185 (parabola weapon update)
=======
class ULockonComponent;

>>>>>>> e7a41e7 (update)
UCLASS()
class TEAMASSIGNMENTFPS_API AParabolaWeapon : public AProjectileWeaponBase
{
	GENERATED_BODY()

public:
	AParabolaWeapon();

protected:
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> e7a41e7 (update)

	UPROPERTY()
	ULockonComponent* LockonComponent=nullptr;// to get the landing location
	
<<<<<<< HEAD
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
>>>>>>> e7a41e7 (update)
	/** Charging and input */
	bool bIsCharging = false;
	float CurrentChargeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Charging")
	float MaxChargeTime = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Charging")
	float MinChargeTime = 0.2f;

	/** Parabola settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MinThrowDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MaxThrowDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MinParabolaHeight = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MaxParabolaHeight = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	int32 PathSegments = 24;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	FColor PathColor = FColor::Green;

protected:
<<<<<<< HEAD
<<<<<<< HEAD
	virtual void BeginPlay() override;
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
	virtual void BeginPlay() override;
>>>>>>> d159577 (weapon update)
	virtual void Tick(float DeltaTime) override;

	/** InputReactionInterface */
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;

	/** Fire the parabola projectile */
	void LaunchParabolaProjectile();

	/** Draw debug path */
	void DrawParabolaPath(float ChargeRatio);
};
