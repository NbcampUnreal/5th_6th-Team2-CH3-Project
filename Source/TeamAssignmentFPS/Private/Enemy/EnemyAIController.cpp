// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Debug/UELOGCategories.h"
#include "BrainComponent.h"

AEnemyAIController::AEnemyAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	//EnemyState = EEnemyState::EES_Idle;

	DefaultSettingBlackBoard();
	StartBehaviorTree();
	


}

void AEnemyAIController::DefaultSettingBlackBoard()
{
	AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(GetPawn());
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!Enemy)
	{
		return;
	}
	if (!Player)
	{
		return;
	}

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Player);
		//BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), static_cast<uint8>(Enemy->GetEnemyState()));
		BlackboardComp->SetValueAsBool(TEXT("CanChasingTarget"), true);
		BlackboardComp->SetValueAsFloat(TEXT("EnemyAttackRange"), Enemy->GetEnemyData().Range);
	}

	Enemy->OnEnemyStateChanged.BindLambda([this](EEnemyState NewState)
	{
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), (uint8)NewState);
			//UE_LOG(Enemy_Log, Error, TEXT("EnemyStateChanged"));
		}

		if (NewState == EEnemyState::EES_Dead)
		{
			StopBehaviorTree();
		}

	});

}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	//UE_LOG(Enemy_Log, Error, TEXT("Move Completed"));

	AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(GetPawn());

	
	Enemy->EnemyAttack();
	
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
}

void AEnemyAIController::StopBehaviorTree()
{
	/*if (BehaviorTreeAsset)
	{
		

		StopBehaviorTree();
	}*/

	

	if (UBrainComponent* Brain = BrainComponent)
	{
		Brain->StopLogic(TEXT("Dead"));
		UE_LOG(Enemy_Log, Warning, TEXT("Stop Behavior tree"));
		
	}
}

