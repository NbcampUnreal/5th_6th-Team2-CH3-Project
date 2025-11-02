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
	
	EnemyData.Range = 1000.f;

}

void ARangeEnemyCharacter::EnemyAttack()
{
	Super::EnemyAttack();

	if (!Projectile || !ProjectileSpawn)
	{
		return;
	}

	UE_LOG(Enemy_Log, Error, TEXT("Range Attack"));
	LookAtPlayer();
	//GetWorld()->SpawnActor<AActor>(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		PoolingSubsystem->SpawnFromPool(Projectile, ProjectileSpawn->GetComponentLocation(), ProjectileSpawn->GetComponentRotation());
	}
	
}

void ARangeEnemyCharacter::LookAtPlayer()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!Player)
	{
		return;
	}

	FRotator TargetRotation	= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
	TargetRotation.Roll = 0.f;
	TargetRotation.Pitch = 0.f;

	SetActorRotation(TargetRotation);
}


