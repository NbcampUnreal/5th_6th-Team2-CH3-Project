// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeaponBase.h"

#include "Animation/AnimInstance.h"
#include "Character/MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"



// Sets default values
AProjectileWeaponBase::AProjectileWeaponBase()
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

	

}

// Called when the game starts or when spawned
void AProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	
	//Set the value
	CurrentAmmoCount=MaxAmmoCount;// set the count
}

// Called every frame
void AProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeaponBase::FireWeapon()
{
	if (!ProjectileClass)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AProjectileWeaponBase::FireWeapon -> No ProjectileClass tp Spawn."));
		return;
	}
	if (bIsReloading)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("AProjectileWeaponBase::FireWeapon -> Cannot fire while reloading."));
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);// no longer cannot fire the weapon
		// TODO:UI-> signal ui manager to show fire failed
		return;
	}
	if (CurrentAmmoCount<=0)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("AProjectileWeaponBase::FireWeapon -> Not enough amo to shoot."));
		ReloadWeapon();// auto reload 
		GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);//same here
		// TODO:UI-> signal ui manager to show fire failed
		
		return;
	}

	FVector SpawnLocation = Muzzle->GetComponentLocation();
	FRotator SpawnRotation = Muzzle->GetComponentRotation();

	FActorSpawnParameters SpawnParams;

	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	

	AProjectileBase* SpawnedProjectile = SpawnProjectile<AProjectileBase>(true, SpawnLocation, SpawnRotation);
	if (SpawnedProjectile)
	{
		SpawnedProjectile->SetDamageInfo(DamageInfo);
	}

	--CurrentAmmoCount;
	PlayMuzzleEffect();
}

void AProjectileWeaponBase::AutoFire(float FireInterval)
{
}

void AProjectileWeaponBase::ReloadWeapon()
{
	if (bIsReloading)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("AWeaponBase::ReloadWeapon -> Already fire while reloading."));
		return;
	}

	bIsReloading = true;

	PlayReloadEffect();
	FTimerHandle ReloadTimerHandle;
	GetWorldTimerManager().SetTimer(ReloadTimerHandle,
		[this]()// do this after ReloadTime
		{
			CurrentAmmoCount = MaxAmmoCount;
			bIsReloading = false;

		}, ReloadTime, false);

	// directly call the function to the character
	//WeaponOwner->// do the thing when reloading is done
}

void AProjectileWeaponBase::PlayMuzzleEffect()
{
}

void AProjectileWeaponBase::PlayReloadEffect()
{
}

void AProjectileWeaponBase::PlayFiringFailedEffect()
{
}

void AProjectileWeaponBase::OnReloadInputPressed_Implementation()
{
	IWeaponInterface::OnReloadInputPressed_Implementation();
	ReloadWeapon();
}

void AProjectileWeaponBase::OnEquipped_Implementation()// put the owner here,
{
	IEquipmentInterface::OnEquipped_Implementation();

	// Set the owner
	AActor* OwnerActor=GetOwner();
	WeaponOwner=Cast<ACharacter>(OwnerActor);
	if (!WeaponOwner)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AProjectileWeaponBase::OnEquipped_Implementation-> Cannot find valid owner"));
	}
	UE_LOG(Weapon_Log, Log, TEXT("AProjectileWeaponBase::OnEquipped_Implementation-> valid owner found"));
}

void AProjectileWeaponBase::OnUnequipped_Implementation()
{
	IEquipmentInterface::OnUnequipped_Implementation();
	WeaponOwner = nullptr;// no more master dobby is fucking freeasldjk;afdafroj asfhoij; faoi; hfewropfesa
}

void AProjectileWeaponBase::SetProjectileInfo()
{
	DamageInfo.DamageCauser=GetInstigator();
}



