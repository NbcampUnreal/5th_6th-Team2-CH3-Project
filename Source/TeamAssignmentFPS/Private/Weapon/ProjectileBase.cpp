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
#include "GameState/GameStateManager.h"
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

	//=== VFX, SFX===//
	ImpactEffect=CreateDefaultSubobject<UParticleSystem>(TEXT("Impact Effect"));
	LifetimeEndEffect=CreateDefaultSubobject<UParticleSystem>(TEXT("LifetimeEnd Effect"));

	ImpactSound=CreateDefaultSubobject<USoundBase>(TEXT("Impact Sound"));
	LifetimeEndSound=CreateDefaultSubobject<USoundBase>(TEXT("LifetimeEnd Sound"));
	
	
	//=== Movement ===//
	MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	// details in the editor

	// start the timer for projectile lifetime
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
	
	AGameStateManager* GameStateManager = Cast<AGameStateManager>(GetWorld()->GetGameState());
	if (GameStateManager)
	{
		UE_LOG(Enemy_Log, Error, TEXT("GameStateManager Found"));
		GameStateManager->PhaseOver.AddDynamic(this, &AProjectileBase::DestroyProjectileAfterLifetime);
	}
	
	DeactivateProjectileBase();
}

void AProjectileBase::DestroyProjectile()
{
	//TODO: make trail to disappear faster when the projectile is dead

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
			TransprotedDamageInfo.DamageDirection = GetVelocity().GetSafeNormal();
			
			HealthComp->GetDamage_Implementation(TransprotedDamageInfo);
		}
	}
	// put direction value to damage info

	DamageInfo.DamageDirection=MovementComponent->Velocity.GetSafeNormal();// now the projectile has the direction value when hit



	//---- VFX, SFX ----//
	
	if (!ImpactEffect)
	{
		UE_LOG(Weapon_Log, Warning, TEXT("AProjectileBase::OnProjectileHit-> No VFX for hit."));
		//no valid effect --> add log category for weapon
	}
	else
	{
		UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		ImpactEffect,//effect to spawn
		HitResult.ImpactPoint,//spawn tranformation: locaton
		HitResult.ImpactNormal.Rotation());//spawn tranformation: rotation
	}
	

	if (!ImpactSound)
	{
		//no sound effect
		UE_LOG(Weapon_Log, Warning, TEXT("AProjectileBase::OnProjectileHit-> No SFX for hit."));
	}
	else
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
	}
	

	DestroyProjectile();
}

void AProjectileBase::ReturnToPool()
{
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		bool WasDestoryed;
		if (!PoolingSubsystem->ReturnToPoolOrDestroy(this,WasDestoryed))
		{
			UE_LOG(Weapon_Log, Error, TEXT("AProjectileBase::ReturnToPool-> Failed to return or destroy"));
			return;
		}

		FString LogString=WasDestoryed? TEXT("is Destroyed"):TEXT("has returned to pool");

		UE_LOG(Weapon_Log,Log, TEXT("AProjectileBase::ReturnToPool-> Projectile %s"), *LogString);
	}
}


void AProjectileBase::OnSpawnFromPool_Implementation()
{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	ActivateProjectileBase();
=======

	MovementComponent->SetUpdatedComponent(this->GetRootComponent());
	MovementComponent->Velocity = GetActorForwardVector() * MovementComponent->InitialSpeed;

=======
>>>>>>> 0f253c7 (Reapply "murge into seo")
	ActivateLifeTimeHandle();
>>>>>>> cccbfc2 (Revert "murge into seo")
=======
	ActivateProjectileBase();
>>>>>>> f79331c (weapon updated)
}

void AProjectileBase::OnReturnToPool_Implementation()
{
	DeactivateProjectileBase();
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
		GetWorldTimerManager().SetTimer(
			LifeTimeHandle,
			this,
			&AProjectileBase::DestroyProjectileAfterLifetime,
			LifeTime,
			false
			);
	});
}
void AProjectileBase::DeactivateLifeTimerHandle()
{
	if (LifeTimeHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(LifeTimeHandle);
	}
}

void AProjectileBase::ActivateProjectileBase()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->ResetInterpolation();
		
		MovementComponent->SetUpdatedComponent(this->GetRootComponent());
		MovementComponent->Velocity=GetActorForwardVector() * MovementComponent->InitialSpeed;
		MovementComponent->Activate(true);
	}
	else
	{
		UE_LOG(Weapon_Log, Error, TEXT(" AProjectileBase::ActivateProjectileBase-> Invalid MovementComp"));
	}

	ActivateLifeTimeHandle();//restart the life timer

	if (TrailComponent)// trail comp
	{
		TrailComponent->Activate(true);
	}
}

void AProjectileBase::DeactivateProjectileBase()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->Deactivate();
		MovementComponent->SetUpdatedComponent(nullptr);
	}

	DeactivateLifeTimerHandle();

	if (TrailComponent)// trail comp
	{
		TrailComponent->Deactivate();
	}
}


