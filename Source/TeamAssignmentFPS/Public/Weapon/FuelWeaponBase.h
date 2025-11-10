// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DamageInfo.h"
#include "Interface/EquipmentInterface.h"
#include "Interface/InputReactionInterface.h"
#include "Interface/WeaponInterface.h"

#include "FuelWeaponBase.generated.h"

// this is the weapon that use the fuel as a amo. not int, but scalable float value
// ex. flame thrower, lazer

UCLASS()
class TEAMASSIGNMENTFPS_API AFuelWeaponBase :
	public AActor,
	public IInputReactionInterface,//for basic input reaction
	public IWeaponInterface,// for reloading
	public IEquipmentInterface/* for equipping and unequipping */

{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFuelWeaponBase();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon | Owner")
	TObjectPtr<ACharacter> WeaponOwner=nullptr;// so that the weapon can trigger specific animation or effect from the owenr character

	//==== Projectile ====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	FDamageInfo DamageInfo;// so that the projectile can get the damage from the weapon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	TObjectPtr<USceneComponent> Muzzle;// location, direction of weapon to be fired

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Projectile")
	float ReloadTime = 1.5f;// required time reload

	
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
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Weapon
	virtual void OnReloadInputPressed_Implementation() override;
	//Input Reaction
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;
	//Equipment
	virtual void OnEquipped_Implementation() override;
	virtual void OnUnequipped_Implementation() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void FireWeapon();// while it is clicked
	virtual void StopFiringWeapon();// stop the 
};
