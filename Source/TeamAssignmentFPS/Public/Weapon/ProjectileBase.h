// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/DamageInfo.h"// added for damage info transport

#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class USphereComponent;// collision detection
class UParticleSystem;// VFX Impact
class UNiagaraSystem;// VFX Trace
class UNiagaraComponent;
class USoundBase;// SFX

UCLASS()
class TEAMASSIGNMENTFPS_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	TObjectPtr<UProjectileMovementComponent> MovementComponent;// this is how projectile move;
	// apply the data from the weapon that spawned this
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile")
	TObjectPtr<USphereComponent> CollisionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FDamageInfo DamageInfo;// put the damage info in here in the editor

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float LifeTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile | VFX")
	TObjectPtr<UParticleSystem> ImpactEffect;//Impact VFX

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile | VFX")
	TObjectPtr<UParticleSystem> LifetimeEndEffect;//Impact VFX when projectile disappear after lifetime
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile | VFX")
	TObjectPtr<UNiagaraSystem> TrailEffect;//Weapon Trail VFX

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Projectile | VFX")
	TObjectPtr<UNiagaraComponent> TrailComponent;//where trail effect come from

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile | SFX")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile | SFX")
	TObjectPtr<USoundBase> LifetimeEndSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;//basically when it spawned

	void DestroyProjectile();
	void DestroyProjectileAfterLifetime();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandleProjectileHit(UPrimitiveComponent* HitComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit);
	
	virtual void OnProjectileHit(const FHitResult& HitResult, AActor* OtherActor);// when the projectile collide

	
};
