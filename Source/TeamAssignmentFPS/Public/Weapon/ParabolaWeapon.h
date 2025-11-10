// ProjectileWeaponParabola.h
#pragma once

#include "CoreMinimal.h"
#include "Weapon/ProjectileWeaponBase.h"
#include "Parabola_ProjectileBase.h"
#include "ParabolaWeapon.generated.h"

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
class ULockonComponent;

=======
>>>>>>> 9dec185 (parabola weapon update)
=======
class ULockonComponent;

>>>>>>> e7a41e7 (update)
=======
=======
>>>>>>> f8cd34c (no message)
class ULockonComponent;

=======
>>>>>>> 5d17d88 (11/10)
<<<<<<< HEAD
>>>>>>> 6018cf4 (no message)
=======
=======
class ULockonComponent;

>>>>>>> cacbb6f (11/10)
>>>>>>> f8cd34c (no message)
UCLASS()
class TEAMASSIGNMENTFPS_API AParabolaWeapon : public AProjectileWeaponBase
{
	GENERATED_BODY()

public:
	AParabolaWeapon();

protected:
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> e7a41e7 (update)
=======
>>>>>>> 6018cf4 (no message)
=======
=======
>>>>>>> cacbb6f (11/10)
>>>>>>> f8cd34c (no message)

	UPROPERTY()
	ULockonComponent* LockonComponent=nullptr;// to get the landing location
	
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
>>>>>>> e7a41e7 (update)
=======
=======
>>>>>>> 5d17d88 (11/10)
>>>>>>> 6018cf4 (no message)
=======
=======
>>>>>>> 5d17d88 (11/10)
=======
>>>>>>> cacbb6f (11/10)
>>>>>>> f8cd34c (no message)
	/** Charging and input */
	bool bIsCharging = false;
	float CurrentChargeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Charging")
	float MaxChargeTime = 2.f;

<<<<<<< HEAD
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Charging")
	float MinChargeTime = 0.2f;

>>>>>>> 5d17d88 (11/10)
	/** Parabola settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
	float MinThrowDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
<<<<<<< HEAD
=======
	float MaxThrowDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola | Settings")
>>>>>>> 5d17d88 (11/10)
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
<<<<<<< HEAD
=======
>>>>>>> 6018cf4 (no message)
	virtual void BeginPlay() override;
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
	virtual void BeginPlay() override;
>>>>>>> d159577 (weapon update)
	virtual void Tick(float DeltaTime) override;
	
	
=======
	virtual void Tick(float DeltaTime) override;

>>>>>>> 5d17d88 (11/10)
	/** InputReactionInterface */
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;

<<<<<<< HEAD
	void FireParabolaProjectile();
	/** Fire the parabola projectile */
	void LaunchParabolaProjectile();

	void TossParabolaProjectile();

=======
	/** Fire the parabola projectile */
	void LaunchParabolaProjectile();

>>>>>>> 5d17d88 (11/10)
	/** Draw debug path */
	void DrawParabolaPath(float ChargeRatio);
};
