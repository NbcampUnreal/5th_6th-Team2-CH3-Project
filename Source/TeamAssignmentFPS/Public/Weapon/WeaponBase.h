// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class AProjectileBase;

UENUM(Blueprintable)
enum class ETriggerInputType:uint8
{
	None UMETA(DisplayName = "None"),// error, no enum
	Presse UMETA(DisplayName = "Presse"),
	Tap UMETA(DisplayName = "Tap"),
	Hold UMETA(DisplayName = "Hold"),
	HoldAndRelease UMETA(DisplayName = "HoldAndRelease"),
};



UCLASS()
class TEAMASSIGNMENTFPS_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:
	
	AWeaponBase();
	
protected:

	//==== Weapon =====//
	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;// weapon mesh with no animation
	
	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;// weapon mesh with animation

	
	//==== Projectile ====//
	UPROPERTY()
	USceneComponent* SpawningLocation;// location, direction of weapon to be fired
	
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
	
	void OnTriggerHolding();
	void OnTriggerReleasedAfterHolding();
	void OnTriggerPressed();
	void OnTriggerTaped();

	void FireBullet();

};
