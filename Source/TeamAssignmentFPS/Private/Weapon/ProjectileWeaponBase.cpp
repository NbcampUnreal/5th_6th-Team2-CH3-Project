// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeaponBase.h"

#include "Weapon/ProjectileBase.h"
#include "Animation/AnimInstance.h"
#include "Character/MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"

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

	CurrentAmmoCount=MaxAmmoCount;// set the count

}

// Called when the game starts or when spawned
void AProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeaponBase::FireWeapon()
{
	if (!Projectile)
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
	
	float FinalDamage = Damage;

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		UObject* SpawnedObj = PoolingSubsystem->SpawnFromPool(Projectile, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
			DamageInfo.DamageAmount = Damage;
			SpawnedProjectile->SetDamageInfo(DamageInfo);
		}
	}
	
	// spawning success	

	// SpawnParams.Owner=this;
	// SpawnParams.Instigator=GetInstigator();
	//SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation, SpawnParams);
	//if (!SpawnedProjectile)
	//{
	//	UE_LOG(Weapon_Log, Error, TEXT("WeaponBase::FireWeapon -> Spawning Projectile Failed."));

	//	return;
	//}


	// spawning success

	CurrentAmmoCount--;//subtract the ammo count
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

void AProjectileWeaponBase::SetProjectileInfo()
{
	DamageInfo.DamageAmount=Damage;
	DamageInfo.DamageCauser=GetInstigator();//
}

void AProjectileWeaponBase::SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!bUsePool)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation, SpawnParams);
		if (!SpawnedProjectile)
		{
			// spawn failed
		}
	}
	else
	{
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
		{
			UObject* SpawnedObj = PoolingSubsystem->SpawnFromPool(Projectile, SpawnLocation, SpawnRotation);
			AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
			if (SpawnedProjectile)
			{
				DamageInfo.DamageAmount = Damage;
				SpawnedProjectile->SetDamageInfo(DamageInfo);
			}
		}
	}
}

