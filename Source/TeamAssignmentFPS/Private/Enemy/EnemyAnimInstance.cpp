// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBaseCharacter.h"


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
	UE_LOG(Enemy_Log, Error, TEXT("SpawnEnd"));
	if (Enemy)
	{
		Enemy->EndEnemySpawn();
	}
}


