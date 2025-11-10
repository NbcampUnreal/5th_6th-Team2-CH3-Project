#include "Weapon/ParabolaWeapon.h"
#include "Weapon/ProjectilePathDrawer.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AParabolaWeapon::AParabolaWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void AParabolaWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharging)
	{
		CurrentChargeTime += DeltaTime;
		float ChargeRatio = FMath::Clamp(CurrentChargeTime / MaxChargeTime, 0.f, 1.f);
		DrawParabolaPath(ChargeRatio);
	}
}


void AParabolaWeapon::OnInputTap_Implementation()
{
	// Quick tap -> fire fast projectile
	bIsCharging = false;
	CurrentChargeTime = MinChargeTime;
	LaunchParabolaProjectile();
}

void AParabolaWeapon::OnInputHoldStart_Implementation()
{
	// Begin holding
	bIsCharging = true;
	CurrentChargeTime = 0.f;
}

void AParabolaWeapon::OnInputHoldUpdate_Implementation(float InputValue)
{
	CurrentChargeTime = FMath::Clamp(CurrentChargeTime + InputValue, 0.f, MaxChargeTime);
}

void AParabolaWeapon::OnInputRelease_Implementation()
{
	// Release -> fire with Stacked charge value
	if (bIsCharging)
	{
		bIsCharging = false;
		LaunchParabolaProjectile();
		CurrentChargeTime = 0.f;
	}
}

void AParabolaWeapon::LaunchParabolaProjectile()
{
	if (!ProjectileClass || !Muzzle)
		return;

	// Compute throw distance based on charge
	float ChargeRatio = FMath::Clamp(CurrentChargeTime / MaxChargeTime, 0.f, 1.f);
	float ThrowDistance = FMath::Lerp(MinThrowDistance, MaxThrowDistance, ChargeRatio);
	float ApexHeight = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	FVector SpawnLocation = Muzzle->GetComponentLocation();
	FRotator SpawnRotation = Muzzle->GetComponentRotation();
	FVector TargetLocation = SpawnLocation + SpawnRotation.Vector() * ThrowDistance;

	// Spawn the projectile from pool
	AParabola_ProjectileBase* Projectile = SpawnProjectile<AParabola_ProjectileBase>(true, SpawnLocation, SpawnRotation);// use template to spawn different one
	if (Projectile)
	{
		Projectile->SetStartAndEndLocation(SpawnLocation,TargetLocation);
		Projectile->SetMaxHeightForParabola(ApexHeight);

		Projectile->SetDamageInfo(DamageInfo);
	}
}

void AParabolaWeapon::DrawParabolaPath(float ChargeRatio)//temp, debug
{
	if (!Muzzle)
		return;

	float ThrowDistance = FMath::Lerp(MinThrowDistance, MaxThrowDistance, ChargeRatio);
	float ApexHeight = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	FVector StartLocation = Muzzle->GetComponentLocation();
	FVector EndLocation = StartLocation + Muzzle->GetComponentRotation().Vector() * ThrowDistance;

	UProjectilePathDrawer::DrawLerpedArc(GetWorld(), StartLocation, EndLocation, 1.f, StartLocation.Z, StartLocation.Z + ApexHeight, PathSegments, PathColor, 0.f);
}
