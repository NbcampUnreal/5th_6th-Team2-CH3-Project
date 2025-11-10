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

	Enemy->OnEnemyStateChanged.AddLambda([this](EEnemyState NewState)
	{
		if (BlackboardComp)
		{
			BlackboardComp->SetValueAsEnum(TEXT("EnemyState"), (uint8)NewState);
		}
		
	});

}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	//UE_LOG(Enemy_Log, Error, TEXT("Move Completed"));
	
	if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(GetPawn()))
	{
		Enemy->EndChase();
	}
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	//StartBehaviorTree();
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
	//DefaultSettingBlackBoard();
=======
	DefaultSettingBlackBoard();
>>>>>>> f351064 (chore)
=======
	//DefaultSettingBlackBoard();
>>>>>>> b8ccb7b (feat enemy anim)
=======
	DefaultSettingBlackBoard();
>>>>>>> 83cc9c5 (delete)
=======
	//DefaultSettingBlackBoard();
>>>>>>> 652a79a (Revert "delete")
=======
=======
>>>>>>> 4c49a91 (11/11)
=======
>>>>>>> d54ae87 (no message)
	//DefaultSettingBlackBoard();
=======
	DefaultSettingBlackBoard();
>>>>>>> 002d088 (11/10 EnemybaseCharacter)
<<<<<<< HEAD
>>>>>>> 2758b11 (11/11)
=======
=======
	//DefaultSettingBlackBoard();
>>>>>>> 078c989 (11/10)
<<<<<<< HEAD
>>>>>>> 4c49a91 (11/11)
=======
=======
	DefaultSettingBlackBoard();
>>>>>>> 8c88e08 (11/10)
>>>>>>> d54ae87 (no message)
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
	}
}

