// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Parabola_ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h" 
#include "Weapon/ProjectilePathDrawer.h"// for parabola


// Sets default values
AParabola_ProjectileBase::AParabola_ProjectileBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// different setting for parabola projectile
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody); 
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); 

}

// Called when the game starts or when spawned
void AParabola_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AParabola_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bUseProjectileMovement)// if it use custom movement, then use tick
	{
		UpdateParabolaMovement(DeltaTime);
	}
}

void AParabola_ProjectileBase::UpdateParabolaMovement(float DeltaTime)
{
	ElapsedTime += DeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / TravelTime, 0.f, 1.f);

	// Get parabola position
	FVector NewLocation = UProjectilePathDrawer::GetLerpedParabolaPoint(
		StartLocation,
		TargetLocation,
		1.f,                  // height scalar
		StartLocation.Z,
		StartLocation.Z + MaxHeight,
		Alpha
	);

	// this is lerp movement, no collision detected by itself
	// make new one here

	FHitResult HitResult;
	SetActorLocation(NewLocation, true, &HitResult); 

	if (HitResult.bBlockingHit)
	{
		OnProjectileHit(HitResult, HitResult.GetActor());
		return; // stop further movement this tick
	}

	if (bIsDebugDrawOn && PathSegments > 0 && GEngine)
	{
		UProjectilePathDrawer::DrawLerpedArc(
			GetWorld(),
			StartLocation,
			TargetLocation,
			1.f,
			StartLocation.Z,
			StartLocation.Z + MaxHeight,
			PathSegments,
			PathColor,
			2.f
		);
	}

	// Check if reached target
	if (Alpha >= 1.f)
	{
		DestroyProjectile();
	}
}

void AParabola_ProjectileBase::ActivateProjectileBase()
{
	// Call base to handle pooling, collision, timer, and trail
	Super::ActivateProjectileBase();

	// Custom setup for parabola movement
	StartLocation = GetActorLocation();
	ElapsedTime = 0.f;

	if (!bUseProjectileMovement)
	{
		// Keep TargetLocation and TravelTime set by weapon
		if (MovementComponent)
		{
			MovementComponent->StopMovementImmediately();
			MovementComponent->Deactivate();
		}
	}
	else
	{
		// If using ProjectileMovement (toss), let it stay active
		if (MovementComponent)
		{
			MovementComponent->bShouldBounce = true;
			MovementComponent->bInitialVelocityInLocalSpace = false;
			MovementComponent->ProjectileGravityScale = 1.f;
			MovementComponent->bSimulationEnabled = true;
			MovementComponent->SetUpdatedComponent(CollisionComponent);
            
			// Make sure it stops on impact
			MovementComponent->bShouldBounce = true;
			MovementComponent->bSweepCollision = true;
			  
		}
	}
}

void AParabola_ProjectileBase::DeactivateProjectileBase()
{
	// Call base to handle pooling deactivation
	Super::DeactivateProjectileBase();

	// Reset parabola movement state
	ElapsedTime = 0.f;
}
