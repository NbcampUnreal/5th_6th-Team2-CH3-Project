// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/FuelWeaponBase.h"

#include "Debug/UELOGCategories.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFuelWeaponBase::AFuelWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(RootComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComponent->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void AFuelWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentFuel = MaxFuel;
}

void AFuelWeaponBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AFuelWeaponBase::OnInputHoldStart_Implementation()
{
	if (CurrentFuel > MinFuelToFire)
	{
		bIsFiring = true;
		TimeSinceLastFire = 0.f;
		FireWeapon();
	}
	else
	{
		PlayFiringFailedEffect();
	}
}


void AFuelWeaponBase::OnInputRelease_Implementation()
{
	StopFiringWeapon();
}

void AFuelWeaponBase::OnEquipped_Implementation()
{
	// Set the owner
	AActor* OwnerActor=GetOwner();
	WeaponOwner=Cast<ACharacter>(OwnerActor);
	if (!WeaponOwner)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AFuelWeaponBase::OnEquipped_Implementation-> Cannot find valid owner"));
	}
	UE_LOG(Weapon_Log, Log, TEXT("AFuelWeaponBase::OnEquipped_Implementation-> valid owner found"));
}

void AFuelWeaponBase::OnUnequipped_Implementation()
{
	WeaponOwner = nullptr;
}

void AFuelWeaponBase::ConsumeFuel(float DeltaSeconds)
{
	if (CurrentFuel <= 0.f)
	{
		StopFiringWeapon();
		return;
	}

	const float Consumed = FuelConsumptionRate * DeltaSeconds;
	CurrentFuel = FMath::Max(CurrentFuel - Consumed, 0.f);
	TimeSinceLastFire = 0.f;

	if (CurrentFuel <= 0.f)
	{
		StopFiringWeapon();
	}
	else
	{
		PlayMuzzleEffect();
	}
}

void AFuelWeaponBase::RecoverFuel(float DeltaSeconds)
{
	if (CurrentFuel < MaxFuel)
	{
		CurrentFuel = FMath::Min(CurrentFuel + FuelRecoveryRate * DeltaSeconds, MaxFuel);
	}
}

void AFuelWeaponBase::PlayMuzzleEffect()
{
}

void AFuelWeaponBase::PlayReloadEffect()
{
}

void AFuelWeaponBase::PlayFiringFailedEffect()
{
}

// Called every frame
void AFuelWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFiring)
	{
		ConsumeFuel(DeltaTime);
	}
	else
	{
		TimeSinceLastFire += DeltaTime;
		if (TimeSinceLastFire >= RecoveryPauseTime)
		{
			RecoverFuel(DeltaTime);
		}
	}

	// update fuel alpha value
	FuelAlpha=FMath::Clamp((CurrentFuel / MaxFuel),0.f, 1.f);
}

void AFuelWeaponBase::FireWeapon()
{
	if (CurrentFuel <= MinFuelToFire)
	{
		PlayFiringFailedEffect();
		StopFiringWeapon();
		return;
	}

	/*PlayMuzzleEffect();
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());*/
}

void AFuelWeaponBase::StopFiringWeapon()
{
	bIsFiring = false;
	TimeSinceLastFire = 0.f;
}

