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
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 652a79a (Revert "delete")
=======
>>>>>>> d54ae87 (no message)
=======
=======
>>>>>>> 4237dcd (11/10)
>>>>>>> 2bf2f5b (no message)
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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 4c49a91 (11/11)
=======
>>>>>>> 2bf2f5b (no message)
=======
>>>>>>> 6a4e438 (no message)

	FVector SpawnDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
=======
	ProjectileSpawn->SetupAttachment(RootComponent);
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
=======
=======

	FVector SpawnDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
=======
>>>>>>> b24b0c4 (no message)
	DamageInfo.DamageCauser=this;// damage causer

	// Projectile path direction
	FVector ProjectileDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
<<<<<<< HEAD
>>>>>>> f79331c (weapon updated)
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======

	FVector SpawnDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
>>>>>>> 9dec185 (parabola weapon update)
=======
>>>>>>> b64879a (weapon updated)
<<<<<<< HEAD
>>>>>>> b24b0c4 (no message)
=======
=======
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
	
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 5e79537 (no message)
		//PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
>>>>>>> b24b0c4 (no message)
=======
>>>>>>> 6a4e438 (no message)
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>
		(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), SpawnDirection.Rotation()));
<<<<<<< HEAD
>>>>>>> 9dec185 (parabola weapon update)
=======
>>>>>>> 4c49a91 (11/11)
=======
=======
	ProjectileSpawn->SetupAttachment(RootComponent);
>>>>>>> 8c88e08 (11/10)
=======
>>>>>>> 4237dcd (11/10)
	
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> cccbfc2 (Revert "murge into seo")
=======
=======
>>>>>>> 54e6a7b (no message)
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
=======
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>
		(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), SpawnDirection.Rotation()));
>>>>>>> 9dec185 (parabola weapon update)
=======
=======
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
>>>>>>> 0c7c848 (no message)
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> 078c989 (11/10)
<<<<<<< HEAD
>>>>>>> 4c49a91 (11/11)
=======
=======
		//PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> 7d3e656 (11/10 enemybasecharacter)
<<<<<<< HEAD
>>>>>>> 54e6a7b (no message)
=======
=======
=======
>>>>>>> f59b66a (11/10)
		AProjectileBase* ProjectileBase =
			Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileDirection.Rotation()));

		if (!ProjectileBase)
		{
			//error
		}
<<<<<<< HEAD
>>>>>>> b64879a (weapon updated)
<<<<<<< HEAD
>>>>>>> b24b0c4 (no message)
=======
=======
=======
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> abfb1f3 (update)
<<<<<<< HEAD
>>>>>>> 035f9ef (11/10)
<<<<<<< HEAD
>>>>>>> 5e79537 (no message)
=======
=======
=======
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
>>>>>>> f59b66a (11/10)
>>>>>>> 6a4e438 (no message)
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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 83cc9c5 (delete)
=======
=======
<<<<<<< HEAD
>>>>>>> 4c49a91 (11/11)
=======
=======
>>>>>>> 2bf2f5b (no message)
=======
<<<<<<< HEAD
=======
<<<<<<< HEAD
=======
>>>>>>> 83cc9c5 (delete)
>>>>>>> 8c88e08 (11/10)
>>>>>>> d54ae87 (no message)
	LookAtPlayer();
	//GetWorld()->SpawnActor<AActor>(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 4c49a91 (11/11)
=======
>>>>>>> d54ae87 (no message)
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
=======
>>>>>>> b8ccb7b (feat enemy anim)
<<<<<<< HEAD
<<<<<<< HEAD
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
=======
>>>>>>> 078c989 (11/10)
>>>>>>> 4c49a91 (11/11)
=======
>>>>>>> 078c989 (11/10)
=======
=======
		PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
>>>>>>> 83cc9c5 (delete)
<<<<<<< HEAD
>>>>>>> 8c88e08 (11/10)
<<<<<<< HEAD
>>>>>>> d54ae87 (no message)
=======
=======
=======
>>>>>>> 652a79a (Revert "delete")
>>>>>>> 4237dcd (11/10)
>>>>>>> 2bf2f5b (no message)
	
}



