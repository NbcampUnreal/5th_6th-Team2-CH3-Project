// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Item_Weapon_Interface.h"

#include "WeaponBase.generated.h"

class AProjectileBase;

UCLASS()
class TEAMASSIGNMENTFPS_API AWeaponBase : public AActor, public IItem_Weapon_Interface
{
	GENERATED_BODY()
	
public:
	
	AWeaponBase();
	
protected:

	//==== Weapon =====//
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;// weapon mesh with no animation
	
	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;// weapon mesh with animation

	
	//==== Projectile ====//
	UPROPERTY()
	TObjectPtr<USceneComponent> SpawningLocation;// location, direction of weapon to be fired
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Weapon | Projectile")
	TSubclassOf<AProjectileBase> TargetActors;// projectile to be fired

	UPROPERTY(EditAnywhere)
	float ReloadTime;// required time reload

	UPROPERTY(EditAnywhere)
	float MaxAmmoCount;//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon | Projectile")
	float CurrentAmmoCount;

	bool bIsReloading=false;
	bool bIsFiring=false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleTriggerInput_Implementation(ETriggerInputType InputType, bool bIsPressed) override;
	
	void OnTriggerHolding();
	void OnTriggerReleasedAfterHolding();
	void OnTriggerPressed();
	void OnTriggerTaped();

	void FireBullet();

};
