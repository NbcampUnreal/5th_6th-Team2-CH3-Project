// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Weapon/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Debug/UELOGCategories.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(FName("Root"));
	
	SkeletalMeshComponent=CreateDefaultSubobject<USkeletalMeshComponent>(FName("Weapon SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(FName("Weapon StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle location"));// where bullet is spawned
	Muzzle->SetupAttachment(SkeletalMeshComponent);

	CurrentAmmoCount=MaxAmmoCount;// set the count
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::OnReloadInputPressed_Implementation()
{
	IWeaponInterface::OnReloadInputPressed_Implementation();

	ReloadWeapon();
}

void AWeaponBase::OnInputPressed_Implementation()
{
	IInputReactionInterface::OnInputPressed_Implementation();
	
	FireWeapon();
}

void AWeaponBase::OnInputTap_Implementation()
{
	IInputReactionInterface::OnInputTap_Implementation();
	// tap -->
}

void AWeaponBase::OnInputHoldStart_Implementation()
{
	IInputReactionInterface::OnInputHoldStart_Implementation();
	
	if (bIsReloading) return;
	
	bIsFiring=true;

	GetWorldTimerManager().
	SetTimer(AutoFireTimerHandle,this,&AWeaponBase::FireWeapon,SpawnInterval,true,0.f);
}

void AWeaponBase::OnInputHoldUpdate_Implementation(float InputValue)
{
	IInputReactionInterface::OnInputHoldUpdate_Implementation(InputValue);
}


void AWeaponBase::OnInputRelease_Implementation()
{
	IInputReactionInterface::OnInputRelease_Implementation();
	bIsFiring=false;

	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}

void AWeaponBase::FireWeapon()
{
	if (!Projectile)
	{
		UE_LOG(Weapon_Log, Error, TEXT("WeaponBase::FireWeapon -> No ProjectileClass tp Spawn."));
		return;
	}
	if (bIsReloading)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("WeaponBase::FireWeapon -> Cannot fire while reloading."));
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);// no longer cannot fire the weapon
		// TODO:UI-> signal ui manager to show fire failed
		return;
	}
	if (CurrentAmmoCount<=0)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("WeaponBase::FireWeapon -> Not enough amo to shoot."));
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);//same here
		// TODO:UI-> signal ui manager to show fire failed
		
		return;
	}

	FVector SpawnLocation=Muzzle->GetComponentLocation();
	FRotator SpawnRotation=Muzzle->GetComponentRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner=this;
	SpawnParams.Instigator=GetInstigator();

	AProjectileBase* SpawnedProjectile=GetWorld()->SpawnActor<AProjectileBase>(Projectile,SpawnLocation,SpawnRotation,SpawnParams );
	if (!SpawnedProjectile)
	{
		UE_LOG(Weapon_Log, Error, TEXT("WeaponBase::FireWeapon -> Spawning Projectile Failed."));
		return;
	}
	
	// spawning success
	CurrentAmmoCount--;//subtract the ammo count
	PlayMuzzleEffect();
	PlayFiringFailedEffect();
}

void AWeaponBase::ReloadWeapon()
{
	if (bIsReloading)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("AWeaponBase::ReloadWeapon -> Already fire while reloading."));
		return;
	}
	
	bIsReloading=true;

	PlayReloadEffect();
	FTimerHandle ReloadTimerHandle;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle,
		[this]()// do this after ReloadTime
	{
		CurrentAmmoCount = MaxAmmoCount;
		bIsReloading = false;
		
	}, ReloadTime, false);
}

void AWeaponBase::PlayMuzzleEffect()
{
}

void AWeaponBase::PlayReloadEffect()
{
}

void AWeaponBase::PlayFiringFailedEffect()
{
}
