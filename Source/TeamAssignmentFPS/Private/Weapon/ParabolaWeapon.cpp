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
}

void AParabolaWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Temp checking
	
	/*if (LockonComponent && WeaponOwner)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("lock on comp and weapon owner is valid"));
	}
	else
	{
		UE_LOG(Weapon_Log, Error, TEXT("lock on comp or weapon owner is invalid"));
	}*/
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

	//Set Timer for debug draw
	GetWorld()->GetTimerManager().SetTimer(
	ParabolaDrawTimer,
	this,
	&AParabolaWeapon::DrawParabolaTimerTick,
	ParabolaDrawInterval,
	true
);
}

void AParabolaWeapon::OnInputHoldUpdate_Implementation(float InputValue)
{
	if(!GetWorld())
	{
		// error, cannot get world
		return;
	}
	if (bIsCharging)
	{
		
		CurrentChargeTime = FMath::Min(CurrentChargeTime + GetWorld()->GetDeltaSeconds(), MaxChargeTime);
		float ChargeRatio = CurrentChargeTime / MaxChargeTime;
		
		bIsCharged = (CurrentChargeTime >= MaxChargeTime - KINDA_SMALL_NUMBER);

		
		//Temp
		//UE_LOG(Weapon_Log, Log, TEXT("ChargeRatio=%f"),ChargeRatio)
		// ratio update check
	}
}

void AParabolaWeapon::OnInputRelease_Implementation()
{
	// Release -> fire with Stacked charge value
	bIsCharging = false;
	FireParabolaProjectile();
	CurrentChargeTime = 0.f;//reset

	GetWorld()->GetTimerManager().ClearTimer(ParabolaDrawTimer);//reset the timer for debug draw
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
	FVector TargetLocation =LockonComponent->GetCursorWorldLocation();

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
	if (!LockonComponent)
	{
		UE_LOG(Weapon_Log, Error, TEXT("AParabolaWeapon::DrawParabolaPath-> LockonComp is invalid"));
		return;
	}
	
	FVector StartLocation = Muzzle->GetComponentLocation();
	FVector EndLocation = LockonComponent->GetCursorWorldLocation();
	
	UE_LOG(Weapon_Log, Log, TEXT("AParabolaWeapon::DrawParabolaPath-> CursorWorldLocation: %s"), *EndLocation.ToString());


	float Height = FMath::Lerp(MinParabolaHeight, MaxParabolaHeight, ChargeRatio);

	// Lerp the color

	// Choose color based on charge state
	FColor CurrentColor;
	if (bIsCharged)
	{
		CurrentColor = PathColor_Charged;
	}
	else
	{
		FLinearColor StartColor = FLinearColor(PathColor_Uncharged);
		FLinearColor EndColor   = FLinearColor(PathColor_Charged);
		FLinearColor LerpColor  = FMath::Lerp(StartColor, EndColor, ChargeRatio);

		CurrentColor = LerpColor.ToFColor(true);
	}
	
	UProjectilePathDrawer::DrawLerpedArc(
		GetWorld(),
		StartLocation,
		EndLocation,
		1.f,
		StartLocation.Z,
		StartLocation.Z + Height,
		PathSegments,
		CurrentColor,
		0.f
	);
}

void AParabolaWeapon::DrawParabolaTimerTick()
{
	if (!bIsCharging) return;

	float ChargeRatio = CurrentChargeTime / MaxChargeTime;
	DrawParabolaPath(ChargeRatio);
}

void AParabolaWeapon::OnEquipped_Implementation()
{
	Super::OnEquipped_Implementation();

	// Now WeaponOwner is guaranteed to be valid because parent class sets it
	if (WeaponOwner)
	{
		LockonComponent = WeaponOwner->FindComponentByClass<ULockonComponent>();
		if (LockonComponent)
		{
			UE_LOG(Weapon_Log, Log, TEXT("ParabolaWeapon -> LockonComponent found: %s"), *LockonComponent->GetName());
		}
		else
		{
			UE_LOG(Weapon_Log, Warning, TEXT("ParabolaWeapon -> LockonComponent not found on %s"), *WeaponOwner->GetName());
		}
	}
	else
	{
		UE_LOG(Weapon_Log, Error, TEXT("ParabolaWeapon -> WeaponOwner invalid during OnEquipped"));
	}
}