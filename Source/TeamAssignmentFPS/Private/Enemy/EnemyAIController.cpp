// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "NavigationSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (BlackboardComp && Player)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Player);
		BlackboardComp->SetValueAsBool(TEXT("CanChasingTarget"), true);
	}

	StartBehaviorTree();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Exist BehaviorTree"));
	}
}

