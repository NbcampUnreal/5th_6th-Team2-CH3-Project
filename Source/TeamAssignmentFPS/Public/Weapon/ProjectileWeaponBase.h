// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/WeaponInterface.h"
#include "Interface/DamageInfo.h"
#include "Interface/EquipmentInterface.h"
#include "Pooling/PoolingSubsystem.h"
#include "Weapon/ProjectileBase.h"
#include "Debug/UELOGCategories.h"

#include "ProjectileWeaponBase.generated.h"



class UStaticMeshComponent;
class USkeletalMeshComponent;
class UParticleSystem;
class USoundBase;
class UAnimMontage;


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
	TObjectPtr<ACharacter> WeaponOwner=nullptr;// so that the weapon can trigger specific animation or effect from the owenr character

	//==== Projectile ====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	FDamageInfo DamageInfo;// so that the projectile can get the damage from the weapon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TObjectPtr<USceneComponent> Muzzle;// location, direction of weapon to be fired

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TSubclassOf<AProjectileBase> ProjectileClass=nullptr;// projectile to be fired


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	float ReloadTime = 1.5f;// required time reload

	//==For Rapid Fire while holding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	float SpawnInterval = 0.2f;// time game between bulletshoot when it is using rapid fire

	FTimerHandle AutoFireTimerHandle;// timer handle for looping time setting
	
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

	
	// the anim pair is for playing animation on same trigger
	// ex. fire weapon-> animations are required for player character's fire animation, weapon's recoil animation
	//or weapon reload -> player reload weapon, weapon being reloaded
	// so, the taaray


	
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

	//Equipment
	virtual void OnEquipped_Implementation() override;
	virtual void OnUnequipped_Implementation() override;

	void SetProjectileInfo();

	// use template for different projectile subclass
template<typename T_ProjectileClass = AProjectileBase> // base as default
T_ProjectileClass* SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!ProjectileClass)
	{
		UE_LOG(Weapon_Log, Error, TEXT("SpawnProjectile -> ProjectileClass is null"));
		return nullptr;
	}

	// Safety check: ensure the template type is a subclass of AProjectileBase
	if (!T_ProjectileClass::StaticClass()->IsChildOf(AProjectileBase::StaticClass()))
	{
		UE_LOG(Weapon_Log, Error,
			TEXT("SpawnProjectile -> Invalid template! %s is not a child of AProjectileBase"),
			*T_ProjectileClass::StaticClass()->GetName());
		return nullptr;
	}

	T_ProjectileClass* SpawnedProjectile = nullptr;

	// Case 1: Spawn normally (not using pool)
	if (!bUsePool)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		SpawnedProjectile = GetWorld()->SpawnActor<T_ProjectileClass>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (!SpawnedProjectile)
		{
			UE_LOG(Weapon_Log, Error, TEXT("SpawnProjectile -> Failed to spawn actor normally"));
			return nullptr;
		}
	}
	// Case 2: Try using pooling subsystem
	else
	{
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
		{
			UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(ProjectileClass, SpawnLocation, SpawnRotation);
			SpawnedProjectile = Cast<T_ProjectileClass>(SpawnedObj);

			if (!SpawnedProjectile)
			{
				UE_LOG(Weapon_Log, Error, TEXT("SpawnProjectile -> Failed to get pooled projectile"));
				return nullptr;
			}

			// Reactivate pooled projectile
			SpawnedProjectile->ActivateProjectileBase();
		}
		else
		{
			UE_LOG(Weapon_Log, Error, TEXT("SpawnProjectile -> PoolingSubsystem not found in World"));
			return nullptr;
		}
	}

	//Log successful spawn info
	if (SpawnedProjectile)
	{
		UE_LOG(Weapon_Log, Log,
			TEXT("SpawnProjectile -> Spawned Projectile: %s (Class: %s) | Using Pool: %s"),
			*SpawnedProjectile->GetName(),
			*SpawnedProjectile->GetClass()->GetName(),
			bUsePool ? TEXT("True") : TEXT("False"));
	}
	else
	{
		UE_LOG(Weapon_Log, Warning, TEXT("SpawnProjectile -> SpawnedProjectile is nullptr after spawn attempt"));
	}

	return SpawnedProjectile;
}

};
