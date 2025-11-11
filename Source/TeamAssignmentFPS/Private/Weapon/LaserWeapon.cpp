// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LaserWeapon.h"

#include "CharacterStat/HealthComponent.h"
#include "Debug/UELOGCategories.h"


class UHealthComponent;
// Sets default values
ALaserWeapon::ALaserWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called every frame
void ALaserWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFiring)
	{
		ConsumeFuel(DeltaTime);
		PerformLaserTrace(DeltaTime);
	}
}

void ALaserWeapon::FireWeapon()
{
	if (CurrentFuel <= MinFuelToFire)
	{
		PlayFiringFailedEffect();
		StopFiringWeapon();
		return;
	}

	bIsFiring = true;
	TimeSinceLastFire = 0.0f;

	UE_LOG(Weapon_Log, Log, TEXT("LaserWeapon::FireWeapon -> Laser started firing"));
}

void ALaserWeapon::StopFiringWeapon()
{
	if (!bIsFiring)
		return;

	bIsFiring = false;
	TimeSinceLastFire = 0.0f;

	UE_LOG(Weapon_Log, Log, TEXT("LaserWeapon::StopFiringWeapon -> Laser stopped"));
}

void ALaserWeapon::PerformLaserTrace(float DeltaSeconds)
{
    if (!Muzzle) return;

    FVector StartLocation = Muzzle->GetComponentLocation();
    FVector ForwardVector = Muzzle->GetForwardVector();
    FVector EndLocation = StartLocation + ForwardVector * MaxDistance;

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(this);

    // Set object types to hit: WorldStatic
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

    TArray<FHitResult> HitResults;
    bool bHit = GetWorld()->LineTraceMultiByObjectType(
        HitResults,
        StartLocation,
        EndLocation,
        ObjectQueryParams,
        TraceParams
    );

    // Draw debug line
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 0.1f, 0, 1.f);

    if (bHit)
    {
        for (const FHitResult& Hit : HitResults)
        {
            // Draw debug sphere at hit points
            DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 8.f, 12, FColor::Yellow, false, 0.1f, 0, 1.5f);

            // Optional: Damage static actors if you want (depends on your design)
        }

        // Shorten laser to first hit
        EndLocation = HitResults[0].ImpactPoint;
    }

    // Draw laser line
    DrawDebugLine(GetWorld(), StartLocation, EndLocation, LaserColor, false, -1.f, 0, LaserThickness);
}
void ALaserWeapon::GiveDamageToActor(AActor* HitActor)
{
	if (!HitActor)
	{
		//error, invalid actor
		return;
	}
	
	if (UHealthComponent* HealthComp = HitActor->FindComponentByClass<UHealthComponent>())// give damage to the actor with hp component
	{
		FDamageInfo TransportedDamageInfo = DamageInfo;
		TransportedDamageInfo.DamageCauser = this;
		TransportedDamageInfo.DamageDirection =  Muzzle->GetForwardVector();

		// Apply damage through interface
		HealthComp->GetDamage_Implementation(TransportedDamageInfo);
	}
	// If no health component, nothing
}

