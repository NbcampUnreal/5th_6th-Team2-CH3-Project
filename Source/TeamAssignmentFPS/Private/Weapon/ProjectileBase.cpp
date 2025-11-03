// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileBase.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CharacterStat/HealthComponent.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Pooling/PoolingSubsystem.h"

AProjectileBase::AProjectileBase()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	//=== Collision ===//
	CollisionComponent=CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent=CollisionComponent;// set the root on the sphere collision
	CollisionComponent->InitSphereRadius(10.f);//default setting for sphere collision
	//=== Mesh ===//
	ProjectileMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// the collision component handles the collision, no need for the mesh
	
	//=== Movement ===//
	MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	// details in the editor
	
}


void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionComponent)// bind collision with HandleProjectileHit
	{
		CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::HandleProjectileHit);
	}

	if (TrailEffect)//when the trail effect is valid
	{
		TrailComponent=UNiagaraFunctionLibrary::SpawnSystemAttached(
			TrailEffect,
			RootComponent,//base
			NAME_None,//attachment point name if there is(but not in here)
			FVector::ZeroVector,//offset location
			FRotator::ZeroRotator,//offset rotation
			EAttachLocation::KeepRelativeOffset,//how to attatch
			true//automatic destruction
			);
	}
	
}

void AProjectileBase::DestroyProjectile()
{
	//TODO: make trail to dissapear faster when the projectile is dead

	if (!TrailComponent)
	{
		// no trail component to destroy
	}
	//TrailComponent->Deactivate();

	//Destroy();//destroy the projectile actor

	ReturnToPool();
}

void AProjectileBase::DestroyProjectileAfterLifetime()
{
	if (!LifetimeEndEffect)
	{
		//no valid effect --> add log category for weapon
	}
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		LifetimeEndEffect,//effect to spwawn
		RootComponent->GetComponentLocation(),//location
		RootComponent->GetComponentRotation()//roation
		);

	if (!LifetimeEndSound)
	{
		//no sound effect
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), LifetimeEndSound, RootComponent->GetComponentLocation());
	
	DestroyProjectile();
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::HandleProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor||OtherActor==this) return;

	OnProjectileHit(Hit, OtherActor);
}

void AProjectileBase::OnProjectileHit(const FHitResult& HitResult, AActor* OtherActor)
{

	if (OtherActor)// should other 
	{
		// Find a HealthComponent on the hit actor
		if (UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>())
		{
			// Fill in the DamageInfo
			FDamageInfo TransprotedDamageInfo = DamageInfo;
			TransprotedDamageInfo.DamageCauser =  this;
			
			HealthComp->GetDamage_Implementation(TransprotedDamageInfo);
		}
	}
	
	if (!ImpactEffect)
	{
		//no valid effect --> add log category for weapon
	}
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ImpactEffect,//effect to spawn
		HitResult.ImpactPoint,//spawn tranformation: locaton
		HitResult.ImpactNormal.Rotation());//spawn tranformation: rotation

	if (!ImpactSound)
	{
		//no sound effect
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);

	DestroyProjectile();
}

void AProjectileBase::ReturnToPool()
{
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		PoolingSubsystem->ReturnToPool(this);
	}
}


void AProjectileBase::OnSpawnFromPool_Implementation()
{
	MovementComponent->SetUpdatedComponent(this->GetRootComponent());
	MovementComponent->Velocity = GetActorForwardVector() * MovementComponent->InitialSpeed;

	ActivateLifeTimeHandle();
}

void AProjectileBase::OnReturnToPool_Implementation()
{
	DeactivateTimerHandle();
}



void AProjectileBase::ActivateLifeTimeHandle()
{
	/*FTimerHandle LifeTimeHandle;
GetWorldTimerManager().SetTimer(LifeTimeHandle, this, &AProjectileBase::DestroyProjectileAfterLifetime, LifeTime, false);*/
	//--> this may cause the issue when the actor got spawned during gameplay. 

	// wait for next tick for safety (one tick delay)
	GetWorldTimerManager().SetTimerForNextTick([this]()// make a reservation for lambda to run on next tick
	{
		//next tick-> triggert the set timer function
		LifeTimeHandle;
		GetWorldTimerManager().SetTimer(
			LifeTimeHandle,
			this,
			&AProjectileBase::DestroyProjectileAfterLifetime,
			LifeTime,
			false
			);
	});
}
void AProjectileBase::DeactivateTimerHandle()
{
	if (LifeTimeHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(LifeTimeHandle);
	}
}

