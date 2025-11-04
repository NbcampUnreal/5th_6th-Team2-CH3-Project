// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyGameMode.h"

#include "Controller/PlayerController/MyPlayerController.h"
#include "Character/MyCharacter.h"
#include "pooling/PoolingSubsystem.h"


AMyGameMode::AMyGameMode()
{
	DefaultPawnClass= AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

}

void AMyGameMode::InitializePooling() const
{
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		PoolingSubsystem->InitializePool(MeleeEnemyClass, 10);
		PoolingSubsystem->InitializePool(RangeEnemyClass, 10);
		PoolingSubsystem->InitializePool(PlayerProjectileClass, 10);
		PoolingSubsystem->InitializePool(EnemyProjectileClass, 10);
	}
}
