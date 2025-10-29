// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InputReactionInterface.h"

#include "WeaponBase.generated.h"


UENUM(BlueprintType)
enum class EWeaponType:uint8
{
	None UMETA(DisplayName = "None"),
	
};






class AProjectileBase;

UCLASS()
class TEAMASSIGNMENTFPS_API AWeaponBase : public AActor, public IInputReactionInterface
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Weapon | Projectile")
	TObjectPtr<USceneComponent> SpawningLocation;// location, direction of weapon to be fired
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Weapon | Projectile")
	TSubclassOf<AProjectileBase> TargetActors;// projectile to be fired

	UPROPERTY()
	EWeaponType WeaponType;
	
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
	
	virtual void OnInputPressed_Implementation() override;
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;

};
