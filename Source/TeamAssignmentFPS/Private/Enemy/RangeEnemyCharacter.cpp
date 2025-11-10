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
	ProjectileSpawn->SetupAttachment(RootComponent);
	
<<<<<<< HEAD
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
		AProjectileBase* ProjectileBase = Cast<AProjectileBase>(PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), LookAtPlayer()));
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
=======
>>>>>>> 83cc9c5 (delete)
	LookAtPlayer();
	//GetWorld()->SpawnActor<AActor>(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
<<<<<<< HEAD
		PoolingSubsystem->BringFromPoolOrSpawn(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
=======
>>>>>>> b8ccb7b (feat enemy anim)
=======
		PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
>>>>>>> 83cc9c5 (delete)
	
}



