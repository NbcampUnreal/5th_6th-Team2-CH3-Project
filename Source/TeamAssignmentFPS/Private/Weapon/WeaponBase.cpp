// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"

#include "Weapon/ProjectileBase.h"
#include "Animation/AnimInstance.h"
#include "Character/MyCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundBase.h"
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingSubsystem.h"

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

	WeaponType = EWeaponType::None;// default

	CurrentAmmoCount=MaxAmmoCount;// set the count
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//Cache Owner
	AMyCharacter* CastedOwner=Cast<AMyCharacter>(GetOwner());
	if (!CastedOwner)//getting owner and casting failed
	{
		//error
	}
	else// cache owner done
	{
		WeaponOwner=CastedOwner;
	}
	
	// set amo count
	CurrentAmmoCount = MaxAmmoCount;
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

	//if (bIsReloading) return;

	FireWeapon();
}

void AWeaponBase::OnInputTap_Implementation()
{
	IInputReactionInterface::OnInputTap_Implementation();
	// tap -->
	if (bIsReloading) return;

	if (WeaponType != EWeaponType::Shot) return;

	FireWeapon();
	bIsFiring = true;
}

void AWeaponBase::OnInputHoldStart_Implementation()
{
	IInputReactionInterface::OnInputHoldStart_Implementation();
	
	if (bIsReloading) return;

	if (WeaponType != EWeaponType::AutoShot) return;

	bIsFiring=true;

	GetWorldTimerManager().
	SetTimer(AutoFireTimerHandle,this,&AWeaponBase::FireWeapon,SpawnInterval,true,0.f);
}

void AWeaponBase::OnInputHoldUpdate_Implementation(float InputValue)
{
	IInputReactionInterface::OnInputHoldUpdate_Implementation(InputValue);

	if (bIsReloading) return;

	if (WeaponType != EWeaponType::ChargingShot) return;

	CurrentChargingTime += GetWorld()->GetDeltaSeconds();
	CurrentChargingTime = FMath::Clamp(CurrentChargingTime, 0.f, MaxChargingTime);
}


void AWeaponBase::OnInputRelease_Implementation()
{
	IInputReactionInterface::OnInputRelease_Implementation();
	if (WeaponType == EWeaponType::ChargingShot)
	{
		if (CurrentChargingTime >= MinChargingTime)
		{
			FireWeapon();
		}
	}

	CurrentChargingTime = 0.f;
	bIsFiring=false;

	GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}

void AWeaponBase::OnEquipped_Implementation()
{
	IEquipmentInterface::OnEquipped_Implementation();
}

void AWeaponBase::OnUnequipped_Implementation()
{
	IEquipmentInterface::OnUnequipped_Implementation();
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
		ReloadWeapon();
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

	//AProjectileBase* SpawnedProjectile=GetWorld()->SpawnActor<AProjectileBase>(Projectile,SpawnLocation,SpawnRotation,SpawnParams );
	//if (!SpawnedProjectile)
	//{
		//UE_LOG(Weapon_Log, Error, TEXT("WeaponBase::FireWeapon -> Spawning Projectile Failed."));
		//return;q
	//

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		// SpawnFromPool�� ��ȯ���� �ӽ� ������ ���� �� Cast
		UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(Projectile, SpawnLocation, SpawnRotation);
		AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
		if (SpawnedProjectile)
		{
			DamageInfo.DamageAmount = Damage;
			SpawnedProjectile->SetDamageInfo(DamageInfo);

			SpawnedProjectile->ActivateProjectileBase();// set activation
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

void AWeaponBase::ReloadWeapon()
{

	if (bIsReloading)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("AWeaponBase::ReloadWeapon -> Already reloading."));
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

void AWeaponBase::PlayMuzzleEffect()
{
}

void AWeaponBase::PlayReloadEffect()
{
}

void AWeaponBase::PlayFiringFailedEffect()
{
}

void AWeaponBase::SpawnProjectile(bool bUsePool, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!bUsePool)
	{
		
	}
	else
	{
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
		{
			// SpawnFromPool�� ��ȯ���� �ӽ� ������ ���� �� Cast
<<<<<<< HEAD
			UObject* SpawnedObj = PoolingSubsystem->BringFromPoolOrSpawn(Projectile, SpawnLocation, SpawnRotation);
=======
			UObject* SpawnedObj = PoolingSubsystem->SpawnFromPool(Projectile, SpawnLocation, SpawnRotation);
>>>>>>> 2966a65 (equipment updated)
			AProjectileBase* SpawnedProjectile = Cast<AProjectileBase>(SpawnedObj);
			if (SpawnedProjectile)
			{
				DamageInfo.DamageAmount = Damage;
				SpawnedProjectile->SetDamageInfo(DamageInfo);
			}
		}
	}
}
