// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Enemy/MeleeEnemyCharacter.h"
#include "Enemy/RangeEnemyCharacter.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Enemy)
	{
		Enemy = Cast<AEnemyBaseCharacter>(TryGetPawnOwner());
	}

	if (Enemy)
	{
		EnemyState = Enemy->GetEnemyState();
		Speed = Enemy->GetVelocity().Size();
	}

	if (EnemyState == EEnemyState::EES_Chase)
	{
		//Speed = Enemy->GetVelocity().Size();
	}

	
	//UE_LOG(Enemy_Log, Error, TEXT("%f"),Speed);
}

void UEnemyAnimInstance::AnimNotify_SpawnEnd()
{
	if (Enemy)
	{
		Enemy->EndEnemySpawn();
	}
}

void UEnemyAnimInstance::AnimNotify_AttackEnd()
{
	if (Enemy)
	{
		Enemy->EndEnemyAttack();
	}
}

void UEnemyAnimInstance::AnimNotify_EndHitReact()
{
	if (Enemy)
	{
		Enemy->EndHitReact();
	}
}

void UEnemyAnimInstance::AnimNotify_EndDead()
{
	if (Enemy)
	{
		Enemy->EndEnemyDead();
	}
}

void UEnemyAnimInstance::AnimNotify_EnableCollision()
{
	if (AMeleeEnemyCharacter* MeleeEnemy = Cast<AMeleeEnemyCharacter>(Enemy))
	{
		MeleeEnemy->EnableAttackCollision();
	}
}

void UEnemyAnimInstance::AnimNotify_DisableCollision()
{
	if (AMeleeEnemyCharacter* MeleeEnemy = Cast<AMeleeEnemyCharacter>(Enemy))
	{
		MeleeEnemy->DisableAttackCollision();
	}
}

void UEnemyAnimInstance::AnimNotify_SpawnProjectile()
{
	if (ARangeEnemyCharacter* Range = Cast<ARangeEnemyCharacter>(Enemy))
	{
		Range->SpawnProjectile();
	}
}


