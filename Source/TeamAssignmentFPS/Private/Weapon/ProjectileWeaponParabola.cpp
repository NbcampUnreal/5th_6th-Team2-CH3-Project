#include "Weapon/ParabolaWeapon.h"
#include "Weapon/ProjectilePathDrawer.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Character/MyCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "LockonTarget/LockonComponent.h"


AParabolaWeapon::AParabolaWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

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

void AParabolaWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCharging)
	{
		CurrentChargeTime = FMath::Min(CurrentChargeTime + DeltaTime, MaxChargeTime);
		float ChargeRatio = CurrentChargeTime / MaxChargeTime;
		DrawParabolaPath(ChargeRatio);
	}
}


void AParabolaWeapon::OnInputTap_Implementation()
{
	// Quick tap -> fire fast projectile
	bIsCharging = false;
	TossParabolaProjectile();
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
	bIsCharging = false;
	LaunchParabolaProjectile();
	CurrentChargeTime = 0.f;//reset
}

void AParabolaWeapon::FireParabolaProjectile()
{
	float ChargeRatio = CurrentChargeTime / MaxChargeTime;

	// If released early → toss
	if (ChargeRatio < 1.f)
	{
		TossParabolaProjectile();
	}
	else
	{
		LaunchParabolaProjectile();
	}
}

void AParabolaWeapon::LaunchParabolaProjectile()
{
	if (!ProjectileClass || !LockonComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("LaunchParabolaProjectile -> Missing LockonComponent or ProjectileClass"));
		return;
	}

	FVector SpawnLocation = Muzzle->GetComponentLocation();
	FVector TargetLocation;
	if (!LockonComponent->GetDeprojectedCursorLocation(TargetLocation))
	{
		UE_LOG(Weapon_Log, Error, TEXT(" AParabolaWeapon::DrawParabolaPath-> Deprojection failed. cannot draw path"));
		return;
	}

	AParabola_ProjectileBase* Projectile = SpawnProjectile<AParabola_ProjectileBase>(true, SpawnLocation, FRotator::ZeroRotator);
	if (!Projectile) return;

	float ChargeRatio = CurrentChargeTime / MaxChargeTime;
	float Height = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);
	float TravelTime = FMath::Lerp(0.8f, 1.6f, ChargeRatio);

	Projectile->SetStartAndEndLocation(SpawnLocation, TargetLocation);
	Projectile->SetMaxHeightForParabola(Height);
	Projectile->SetTravelTime(TravelTime);
	Projectile->SetActorLocation(SpawnLocation);

	UE_LOG(LogTemp, Log, TEXT("LaunchParabolaProjectile -> Arc launched toward cursor (%.1fs travel)"), TravelTime);
}

void AParabolaWeapon::TossParabolaProjectile()
{
	if (!ProjectileClass) return;

	FVector SpawnLocation = Muzzle->GetComponentLocation();
	FRotator SpawnRotation = Muzzle->GetComponentRotation();

	AProjectileBase* Projectile = SpawnProjectile<AProjectileBase>(true, SpawnLocation, SpawnRotation);
	if (!Projectile) return;

	if (UProjectileMovementComponent* MoveComp = Projectile->FindComponentByClass<UProjectileMovementComponent>())
	{
		MoveComp->ProjectileGravityScale = 1.f;
		MoveComp->bShouldBounce = true;
		MoveComp->Velocity = SpawnRotation.Vector() * 1500.f;
	}

	UE_LOG(LogTemp, Log, TEXT("TossParabolaProjectile -> Toss launched"));
}

void AParabolaWeapon::DrawParabolaPath(float ChargeRatio)
{
	if (!LockonComponent) return;

	FVector StartLocation = Muzzle->GetComponentLocation();
	FVector EndLocation;
	if (!LockonComponent->GetDeprojectedCursorLocation(EndLocation))
	{
		UE_LOG(Weapon_Log, Error, TEXT(" AParabolaWeapon::DrawParabolaPath-> Deprojection failed. cannot draw path"));
		return;
	}
	// end location set
	
	float Height = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	UProjectilePathDrawer::DrawLerpedArc(
		GetWorld(),
		StartLocation,
		EndLocation,
		1.f,
		StartLocation.Z,
		StartLocation.Z + Height,
		PathSegments,
		PathColor,
		0.f
	);
}
