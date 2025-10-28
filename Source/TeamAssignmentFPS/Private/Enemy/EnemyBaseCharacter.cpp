// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/EnemyBaseCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bOrientRotationToMovement = true;

	Movement->MaxWalkSpeed = 300.f;

	EnemyState = EEnemyState::Idle;

}

// Called when the game starts or when spawned
void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

