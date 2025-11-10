// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/WeaponInterface.h"
#include "Interface/DamageInfo.h"
#include "Interface/EquipmentInterface.h"
#include "ProjectileWeaponBase.generated.h"


class AProjectileBase;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class UAnimMontage;
class AMyCharacter;


UCLASS()
class TEAMASSIGNMENTFPS_API AProjectileWeaponBase :
	public AActor,
	public IInputReactionInterface,//for basic input reaction
	public IWeaponInterface,// for reloading
	public IEquipmentInterface/* for equipping and unequipping */

{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileWeaponBase();
	
protected:
	//Owner
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Owner")
	TObjectPtr<AMyCharacter> WeaponOwner=nullptr;// so that the weapon can trigger specific animation or effect from the owenr character

	//==== Projectile ====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	FDamageInfo DamageInfo;// so that the projectile can get the damage from the weapon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TObjectPtr<USceneComponent> Muzzle;// location, direction of weapon to be fired

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TSubclassOf<AProjectileBase> Projectile=nullptr;// projectile to be fired


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


protected:
	virtual void FireWeapon();
	virtual void AutoFire(float FireInterval);
	virtual void ReloadWeapon();
	virtual void PlayMuzzleEffect();
	virtual void PlayReloadEffect();
	virtual void PlayFiringFailedEffect();

	void SetProjectileInfo();

	void SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation);
};
