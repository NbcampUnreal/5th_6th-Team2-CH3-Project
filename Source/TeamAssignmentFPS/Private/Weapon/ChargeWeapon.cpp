#include "Weapon/ChargeWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundBase.h"

AChargeWeapon::AChargeWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AChargeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AChargeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharging && !bIsChargingStopped)
	{
		UpdateCharging(DeltaTime);
	}
}

void AChargeWeapon::StartCharging()
{
	if (bIsCharging)
		return;

	bIsCharging = true;
	bIsChargingStopped = false;
	CurrentChargeTime = 0.0f;

	// Play VFX 
	if (ChargingEffect && Muzzle)
	{
		UGameplayStatics::SpawnEmitterAttached(ChargingEffect, Muzzle);
	}

	// Play SFX
	if (ChargingSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ChargingSound, GetActorLocation());
	}

}

void AChargeWeapon::UpdateCharging(float DeltaTime)
{
	if (!bIsCharging || bIsChargingStopped)
		return;

	CurrentChargeTime += DeltaTime * ChargeRate;
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime, 0.0f, MaxChargeTime);
}

void AChargeWeapon::StopCharging()
{
	if (!bIsCharging)
		return;

	bIsChargingStopped = true;

	// Could trigger visual feedback (like flickering or dimming)
	UE_LOG(Weapon_Log, Log, TEXT("AChargeWeapon::StopCharging -> Charging paused at %.2f"), CurrentChargeTime);
}

void AChargeWeapon::ReleaseCharge()
{
	if (!bIsCharging)
		return;

	bIsCharging = false;
	bIsChargingStopped = false;

	// Child class handles what happens on release 
	UE_LOG(Weapon_Log, Log, TEXT("AChargeWeapon::ReleaseCharge -> Released with ratio: %.2f"), GetChargeRatio());
}

void AChargeWeapon::ResetCharge()
{
	bIsCharging = false;
	bIsChargingStopped = false;
	CurrentChargeTime = 0.0f;

	// For interrupts, switching weapons, etc.
	UE_LOG(Weapon_Log, Log, TEXT("AChargeWeapon::ResetCharge -> Charge instantly reset."));
}

float AChargeWeapon::GetChargeRatio() const
{
	const float Normalized = FMath::Clamp(CurrentChargeTime / MaxChargeTime, 0.0f, 1.0f);
	return ChargeCurve ? ChargeCurve->GetFloatValue(Normalized) : Normalized;
}

void AChargeWeapon::FireChargedProjectile(float ChargeRatio)
{
	UE_LOG(Weapon_Log, Log, TEXT("AChargeWeapon::FireChargedProjectile -> Fired with ratio: %.2f"), ChargeRatio);
}

bool AChargeWeapon::IsMinChargeSatisfied() const
{
	return CurrentChargeTime >= MinChargeTime;
}
