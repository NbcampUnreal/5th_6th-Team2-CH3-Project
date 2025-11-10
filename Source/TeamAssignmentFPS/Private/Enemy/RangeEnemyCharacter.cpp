// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangeEnemyCharacter.h"
#include "Weapon/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pooling/PoolingSubsystem.h"
#include "Enemy/EnemyAIController.h"
//#include 
ARangeEnemyCharacter::ARangeEnemyCharacter()
{
	EnemyType = EEnemyType::EET_Range;

	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Location"));
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 652a79a (Revert "delete")
	ProjectileSpawn->SetupAttachment(GetMesh(), TEXT("RightHandSocket"));
}

void ARangeEnemyCharacter::SpawnProjectile()
{
	if (!Projectile || !ProjectileSpawn)
	{
		return;
	}

	FDamageInfo DamageInfo;
	DamageInfo.DamageAmount = GetEnemyData().Damage;
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD

	FVector SpawnDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
=======
	ProjectileSpawn->SetupAttachment(RootComponent);
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
=======
	DamageInfo.DamageCauser=this;// damage causer

	// Projectile path direction
	FVector ProjectileDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
>>>>>>> f79331c (weapon updated)
	
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		//PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
<<<<<<< HEAD
<<<<<<< HEAD
		AProjectileBase* ProjectileBase =
			Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileDirection.Rotation()));

		if (!ProjectileBase)
		{
			//error
		}
=======
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
<<<<<<< HEAD
>>>>>>> abfb1f3 (update)
=======
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>
		(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), SpawnDirection.Rotation()));
>>>>>>> 9dec185 (parabola weapon update)
=======
	
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
=======
>>>>>>> cccbfc2 (Revert "murge into seo")
=======
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
>>>>>>> 0f253c7 (Reapply "murge into seo")
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> b8ccb7b (feat enemy anim)
=======
		//PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
<<<<<<< HEAD
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> d74bc04 (fix)
=======
		AProjectileBase* ProjectileBase =
			Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileDirection.Rotation()));

		if (!ProjectileBase)
		{
			//error
		}
>>>>>>> f79331c (weapon updated)
=======
>>>>>>> abfb1f3 (update)
		ProjectileBase->SetDamageInfo(DamageInfo);
	}

	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->SetCanAttackRotate(false);
	}
}

void ARangeEnemyCharacter::EnemyAttack()
{
	Super::EnemyAttack();

	
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 83cc9c5 (delete)
	LookAtPlayer();
	//GetWorld()->SpawnActor<AActor>(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
=======
>>>>>>> b8ccb7b (feat enemy anim)
=======
		PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
=======
>>>>>>> b8ccb7b (feat enemy anim)
=======
		PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
	
}



