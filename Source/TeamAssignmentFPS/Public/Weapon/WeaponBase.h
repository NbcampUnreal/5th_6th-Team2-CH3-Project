// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/WeaponInterface.h"
#include "Interface/DamageInfo.h"

#include "WeaponBase.generated.h"


//forward declare
class AProjectileBase;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class UAnimMontage;


UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	None UMETA(DisplayName = "None"),
	Shot UMETA(DisplayName = "Shot"),
	AutoShot UMETA(DisplayName = "AutoShot"),
	ChargingShot UMETA(DisplayName = "ChargingShot"),
};


UCLASS()
class TEAMASSIGNMENTFPS_API AWeaponBase :
	public AActor,
	public IInputReactionInterface,
	public IWeaponInterface
{
	GENERATED_BODY()

public:

	AWeaponBase();

protected:

	//==== Projectile ====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	FDamageInfo DamageInfo;// so that the projectile can get the damage from the weapon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TObjectPtr<USceneComponent> Muzzle;// location, direction of weapon to be fired

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TSubclassOf<AProjectileBase> Projectile;// projectile to be fired

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType = EWeaponType::None;//default

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	float ReloadTime = 1.5f;// required time reload

	//==For Rapid Fire while holding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	float SpawnInterval = 0.2f;// time game between bulletshoot when it is using rapid fire

	FTimerHandle AutoFireTimerHandle;// timer handle for looping time setting

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	int32 MaxAmmoCount;//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Projectile")
	int32 CurrentAmmoCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	float CurrentChargingTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Charging")
	float MinChargingTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Charging ")
	float MaxChargingTime;

	bool bIsReloading = false;
	bool bIsFiring = false;

	/// Visual elements

	//	Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;// weapon mesh with no animation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Weapon")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;// weapon mesh with animation

	//	FX
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | FX")
	TObjectPtr<UParticleSystem> MuzzleFlashEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | FX")
	TObjectPtr<USoundBase> FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | FX")
	TObjectPtr<USoundBase> ReloadSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | FX")
	TObjectPtr<USoundBase> FiringFailedSound;

	//	when skeletal mesh has animaiton
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Animation")
	TObjectPtr<UAnimMontage> FireAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Animation")
	TObjectPtr<UAnimMontage> ReloadAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Animation")
	TObjectPtr<UAnimMontage> FireFailedAnimMontage;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnReloadInputPressed_Implementation() override;
	virtual void OnInputPressed_Implementation() override;
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;

protected:
	virtual void FireWeapon();
	virtual void ReloadWeapon();
	virtual void PlayMuzzleEffect();
	virtual void PlayReloadEffect();
	virtual void PlayFiringFailedEffect();

};
