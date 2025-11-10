// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangeEnemyCharacter.h"
#include "Weapon/ProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pooling/PoolingSubsystem.h"
//#include 
ARangeEnemyCharacter::ARangeEnemyCharacter()
{
	EnemyType = EEnemyType::EET_Range;

	ProjectileSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Location"));
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 652a79a (Revert "delete")
=======
>>>>>>> d54ae87 (no message)
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
=======
>>>>>>> 4c49a91 (11/11)

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
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======

	FVector SpawnDirection=(TargetActor->GetActorLocation()-GetActorLocation()).GetSafeNormal();
>>>>>>> 9dec185 (parabola weapon update)
	
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		//PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
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
	
<<<<<<< HEAD
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
<<<<<<< HEAD
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
>>>>>>> 4c49a91 (11/11)
		ProjectileBase->SetDamageInfo(DamageInfo);
	}
=======
	EnemyData.Range = 1000.f;
>>>>>>> parent of b8ccb7b (feat enemy anim)

}

void ARangeEnemyCharacter::EnemyAttack()
{
	Super::EnemyAttack();

	if (!Projectile || !ProjectileSpawn)
	{
		return;
	}
	
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
>>>>>>> 8c88e08 (11/10)
>>>>>>> d54ae87 (no message)
	
}



