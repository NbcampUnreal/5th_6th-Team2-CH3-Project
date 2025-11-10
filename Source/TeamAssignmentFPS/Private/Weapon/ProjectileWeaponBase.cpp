// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeaponBase.h"

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> 7568c9b (weapon updated)
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
>>>>>>> a8cc1bd (rebase update)
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> ca434e8 (no message)
>>>>>>> cda4565 (11/11)
=======
=======
>>>>>>> 29523ae (no message)
=======
>>>>>>> 6a4e438 (no message)
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> ca434e8 (no message)
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> ea6a2be (11/10)
<<<<<<< HEAD
>>>>>>> 0c7c848 (no message)
=======
=======
>>>>>>> 616ab73 (11/10)
<<<<<<< HEAD
>>>>>>> 29523ae (no message)
=======
=======
#include "Weapon/ProjectileBase.h"
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
#include "Animation/AnimInstance.h"
#include "Character/MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundBase.h"
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> 29523ae (no message)
=======
>>>>>>> 6a4e438 (no message)


=======
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"
<<<<<<< HEAD
>>>>>>> 7568c9b (weapon updated)
=======
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======


>>>>>>> a8cc1bd (rebase update)
=======
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
>>>>>>> ca434e8 (no message)
<<<<<<< HEAD
>>>>>>> cda4565 (11/11)
=======
=======
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"
>>>>>>> ea6a2be (11/10)
<<<<<<< HEAD
>>>>>>> 0c7c848 (no message)
=======
=======


>>>>>>> 616ab73 (11/10)
<<<<<<< HEAD
>>>>>>> 29523ae (no message)
=======
=======
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)

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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	
=======
	CurrentAmmoCount=MaxAmmoCount;// set the count
>>>>>>> 7568c9b (weapon updated)
=======
	CurrentAmmoCount=MaxAmmoCount;// set the count
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
	
>>>>>>> b028351 (parabola weapon updated)
=======
=======
>>>>>>> 0c7c848 (no message)
	
=======
	CurrentAmmoCount=MaxAmmoCount;// set the count
>>>>>>> ca434e8 (no message)
<<<<<<< HEAD
>>>>>>> cda4565 (11/11)
=======
=======
	CurrentAmmoCount=MaxAmmoCount;// set the count
>>>>>>> ea6a2be (11/10)
>>>>>>> 0c7c848 (no message)

}

// Called when the game starts or when spawned
void AProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> 29523ae (no message)
=======
>>>>>>> 6a4e438 (no message)

	
	//Set the value
	CurrentAmmoCount=MaxAmmoCount;// set the count
=======
	
<<<<<<< HEAD
>>>>>>> 7568c9b (weapon updated)
=======
	
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======

<<<<<<< HEAD
	// Set the owner
	AActor* OwnerActor=GetOwner();
	WeaponOwner=Cast<ACharacter>(OwnerActor);
	if (!WeaponOwner)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AProjectileWeaponBase::BeginPlay-> Cannot find valid owner"));
	}
	UE_LOG(Weapon_Log, Log, TEXT("AProjectileWeaponBase::BeginPlay-> valid owner found"));
>>>>>>> a8cc1bd (rebase update)
=======
	
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
	
	//Set the value
	CurrentAmmoCount=MaxAmmoCount;// set the count
>>>>>>> b028351 (parabola weapon updated)
=======
>>>>>>> ca434e8 (no message)
<<<<<<< HEAD
>>>>>>> cda4565 (11/11)
=======
=======
	
>>>>>>> ea6a2be (11/10)
<<<<<<< HEAD
>>>>>>> 0c7c848 (no message)
=======
=======

	// Set the owner
	AActor* OwnerActor=GetOwner();
	WeaponOwner=Cast<ACharacter>(OwnerActor);
	if (!WeaponOwner)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AProjectileWeaponBase::BeginPlay-> Cannot find valid owner"));
	}
	UE_LOG(Weapon_Log, Log, TEXT("AProjectileWeaponBase::BeginPlay-> valid owner found"));
>>>>>>> 616ab73 (11/10)
<<<<<<< HEAD
>>>>>>> 29523ae (no message)
=======
=======
	
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
}

// Called every frame
void AProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectileWeaponBase::FireWeapon()
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	if (!ProjectileClass)
=======
	if (!Projectile)
>>>>>>> 7568c9b (weapon updated)
=======
	if (!ProjectileClass)
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
=======
>>>>>>> 0c7c848 (no message)
	if (!ProjectileClass)
=======
	if (!Projectile)
>>>>>>> ca434e8 (no message)
<<<<<<< HEAD
>>>>>>> cda4565 (11/11)
=======
=======
	if (!ProjectileClass)
>>>>>>> ea6a2be (11/10)
>>>>>>> 0c7c848 (no message)
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
	
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(ProjectileClass, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
			SpawnedProjectile->SetDamageInfo(DamageInfo);
		}
	}
	
	// spawning success

<<<<<<< HEAD
	--CurrentAmmoCount;
=======
	float FinalDamage = Damage;

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		UObject* SpawnedObj = PoolingSubsystem->SpawnFromPool(Projectile, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
			DamageInfo.DamageAmount = Damage;
=======
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> 0c7c848 (no message)

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 6a4e438 (no message)
		UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(ProjectileClass, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
<<<<<<< HEAD
>>>>>>> 0f253c7 (Reapply "murge into seo")
			SpawnedProjectile->SetDamageInfo(DamageInfo);
		}
	}
	
<<<<<<< HEAD
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
>>>>>>> 7568c9b (weapon updated)
=======
	// spawning success

	CurrentAmmoCount--;//subtract the ammo count
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
		SpawnedProjectile->SetDamageInfo(DamageInfo);
=======
			SpawnedProjectile->SetDamageInfo(DamageInfo);
		}
>>>>>>> 6a4e438 (no message)
	}
	
	// spawning success

<<<<<<< HEAD
	--CurrentAmmoCount;
<<<<<<< HEAD
>>>>>>> a8cc1bd (rebase update)
=======
	CurrentAmmoCount--;//subtract the ammo count
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
=======
	float FinalDamage = Damage;

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		UObject* SpawnedObj = PoolingSubsystem->SpawnFromPool(Projectile, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
			DamageInfo.DamageAmount = Damage;
=======

	AProjectileBase* SpawnedProjectile = SpawnProjectile<AProjectileBase>(true, SpawnLocation, SpawnRotation);
	if (SpawnedProjectile)
	{
<<<<<<< HEAD
		UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(ProjectileClass, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
>>>>>>> ea6a2be (11/10)
			SpawnedProjectile->SetDamageInfo(DamageInfo);
		}
	}
	
<<<<<<< HEAD
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
>>>>>>> ca434e8 (no message)
<<<<<<< HEAD
>>>>>>> cda4565 (11/11)
=======
=======
	// spawning success

	CurrentAmmoCount--;//subtract the ammo count
>>>>>>> ea6a2be (11/10)
<<<<<<< HEAD
>>>>>>> 0c7c848 (no message)
=======
=======
		SpawnedProjectile->SetDamageInfo(DamageInfo);
	}

	--CurrentAmmoCount;
>>>>>>> 616ab73 (11/10)
<<<<<<< HEAD
>>>>>>> 29523ae (no message)
=======
=======
	CurrentAmmoCount--;//subtract the ammo count
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b028351 (parabola weapon updated)
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> 0c7c848 (no message)
void AProjectileWeaponBase::OnEquipped_Implementation()// put the owner here,
{
	IEquipmentInterface::OnEquipped_Implementation();

	// Set the owner
	AActor* OwnerActor=GetOwner();
	WeaponOwner=Cast<ACharacter>(OwnerActor);
	if (!WeaponOwner)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AProjectileWeaponBase::BeginPlay-> Cannot find valid owner"));
	}
	UE_LOG(Weapon_Log, Log, TEXT("AProjectileWeaponBase::BeginPlay-> valid owner found"));
}

void AProjectileWeaponBase::OnUnequipped_Implementation()
{
	IEquipmentInterface::OnUnequipped_Implementation();
}

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
>>>>>>> b028351 (parabola weapon updated)
=======
=======
>>>>>>> ea6a2be (11/10)
>>>>>>> 0c7c848 (no message)
void AProjectileWeaponBase::SetProjectileInfo()
{
	DamageInfo.DamageCauser=GetInstigator();
}

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> 29523ae (no message)
=======
>>>>>>> 6a4e438 (no message)

=======
void AProjectileWeaponBase::SetProjectileInfo()
{
	DamageInfo.DamageAmount=Damage;
	DamageInfo.DamageCauser=GetInstigator();//
}

void AProjectileWeaponBase::SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation)
{
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 0c7c848 (no message)
=======
AProjectileBase* AProjectileWeaponBase::SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation) const
{

	AProjectileBase* SpawnedProjectile=nullptr;
	
<<<<<<< HEAD
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
AProjectileBase* AProjectileWeaponBase::SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation) const
{

	AProjectileBase* SpawnedProjectile=nullptr;
	
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> ea6a2be (11/10)
<<<<<<< HEAD
>>>>>>> 0c7c848 (no message)
=======
=======
AProjectileBase* AProjectileWeaponBase::SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation) const
{

	AProjectileBase* SpawnedProjectile=nullptr;
	
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
	if (!bUsePool)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> 6a4e438 (no message)
		AProjectileBase* SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(Projectile, SpawnLocation, SpawnRotation, SpawnParams);
		if (!SpawnedProjectile)
		{
			// spawn failed
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
<<<<<<< HEAD
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
=======
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
		SpawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
		if (!SpawnedProjectile)
		{
			// spawn failed
			return nullptr;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> ea6a2be (11/10)
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> ea6a2be (11/10)
=======
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
		}
	}
	else
	{
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
		{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> cda4565 (11/11)
=======
>>>>>>> 0c7c848 (no message)
=======
>>>>>>> 6a4e438 (no message)
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
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 7568c9b (weapon updated)
=======
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
>>>>>>> ca434e8 (no message)
=======
<<<<<<< HEAD
>>>>>>> 0c7c848 (no message)
=======
=======
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
			UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(ProjectileClass, SpawnLocation, SpawnRotation);
			SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
			if (!SpawnedProjectile)
			{
				return nullptr;
			}
		}
	}
	// spawning new or from pool completed

	return SpawnedProjectile;
}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> 0f253c7 (Reapply "murge into seo")
=======

>>>>>>> a8cc1bd (rebase update)
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
>>>>>>> ca434e8 (no message)
>>>>>>> cda4565 (11/11)
=======
>>>>>>> ea6a2be (11/10)
>>>>>>> 0c7c848 (no message)
=======
=======
>>>>>>> 6a4e438 (no message)
>>>>>>> ea6a2be (11/10)
=======

>>>>>>> 616ab73 (11/10)
<<<<<<< HEAD
>>>>>>> 29523ae (no message)
=======
=======
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)

