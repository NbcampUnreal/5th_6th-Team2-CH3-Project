#include "Weapon/ParabolaWeapon.h"
#include "Weapon/ProjectilePathDrawer.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
<<<<<<< HEAD
#include "Character/MyCharacter.h"
#include "LockonTarget/LockonComponent.h"

=======
>>>>>>> 9dec185 (parabola weapon update)

AParabolaWeapon::AParabolaWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

<<<<<<< HEAD
void AParabolaWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (AMyCharacter* CharOwner = Cast<AMyCharacter>(GetOwner()))
	{
		LockonComponent = CharOwner->GetLoconComp();
		if (!LockonComponent)
		{
			UE_LOG(Weapon_Log, Warning, TEXT("ParabolaWeapon: No LockonComponent found on owner!"));
		}
	}
}

=======
>>>>>>> 9dec185 (parabola weapon update)
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
<<<<<<< HEAD
	
	FVector SpawnLocation = Muzzle->GetComponentLocation();
	FRotator SpawnRotation = Muzzle->GetComponentRotation();
	
	FVector TargetLocation;
	if (LockonComponent && LockonComponent->GetLockonTarget())
	{
		TargetLocation = LockonComponent->GetLockonTarget()->GetActorLocation();
	}
	else if (LockonComponent)// if lock on is valid
	{
		FVector CursorWorldLocation;
		if (LockonComponent->GetDeprojectedCursorLocation(CursorWorldLocation))// get cursor world location
		{
			TargetLocation = CursorWorldLocation;
		}
		else// couldnt find the location
		{
			TargetLocation = SpawnLocation + SpawnRotation.Vector() * MaxThrowDistance;
		}
	}
	else// lockon comp invalid
	{
		TargetLocation = SpawnLocation + SpawnRotation.Vector() * MaxThrowDistance;
	}
	
	float ChargeRatio = FMath::Clamp(CurrentChargeTime / MaxChargeTime, 0.f, 1.f);
	float ApexHeight = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);
	
=======

	// Compute throw distance based on charge
	float ChargeRatio = FMath::Clamp(CurrentChargeTime / MaxChargeTime, 0.f, 1.f);
	float ThrowDistance = FMath::Lerp(MinThrowDistance, MaxThrowDistance, ChargeRatio);
	float ApexHeight = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	FVector SpawnLocation = Muzzle->GetComponentLocation();
	FRotator SpawnRotation = Muzzle->GetComponentRotation();
	FVector TargetLocation = SpawnLocation + SpawnRotation.Vector() * ThrowDistance;

>>>>>>> 9dec185 (parabola weapon update)
	// Spawn the projectile from pool
	AParabola_ProjectileBase* Projectile = SpawnProjectile<AParabola_ProjectileBase>(true, SpawnLocation, SpawnRotation);// use template to spawn different one
	if (Projectile)
	{
		Projectile->SetStartAndEndLocation(SpawnLocation,TargetLocation);
		Projectile->SetMaxHeightForParabola(ApexHeight);

		Projectile->SetDamageInfo(DamageInfo);
	}
}

<<<<<<< HEAD
void AParabolaWeapon::DrawParabolaPath(float ChargeRatio)
{
	if (!Muzzle || !LockonComponent) return;

	FVector StartLocation = Muzzle->GetComponentLocation();
	FVector TargetLocation;

	if (!LockonComponent->GetDeprojectedCursorLocation(TargetLocation))// if the lock on is invalid
	{
		TargetLocation = StartLocation + Muzzle->GetComponentRotation().Vector() * MaxThrowDistance;// just draw the path with max distance
	}

	float ApexHeight = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	UProjectilePathDrawer::DrawLerpedArc(GetWorld(), StartLocation, TargetLocation, 1.f, StartLocation.Z, StartLocation.Z + ApexHeight, PathSegments, PathColor, 0.f);
=======
void AParabolaWeapon::DrawParabolaPath(float ChargeRatio)//temp, debug
{
	if (!Muzzle)
		return;

	float ThrowDistance = FMath::Lerp(MinThrowDistance, MaxThrowDistance, ChargeRatio);
	float ApexHeight = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	FVector StartLocation = Muzzle->GetComponentLocation();
	FVector EndLocation = StartLocation + Muzzle->GetComponentRotation().Vector() * ThrowDistance;

	UProjectilePathDrawer::DrawLerpedArc(GetWorld(), StartLocation, EndLocation, 1.f, StartLocation.Z, StartLocation.Z + ApexHeight, PathSegments, PathColor, 0.f);
>>>>>>> 9dec185 (parabola weapon update)
}
