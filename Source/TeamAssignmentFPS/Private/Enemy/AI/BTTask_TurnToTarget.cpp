// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTask_TurnToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Debug/UELOGCategories.h"
#include "GameFramework/Character.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Rotate To Target");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	ACharacter* Enemy = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	ACharacter* Player = Cast<ACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));
	if (!Player)
	{
		return EBTNodeResult::Failed;
	}

	//FVector LookVector = Player->GetActorLocation() - Enemy->GetActorLocation();
	FVector LookVector = Player->GetActorLocation() - Enemy->GetMesh()->GetSocketLocation(TEXT("RightHandSocket"));
	
	FRotator PlayerRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

	Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), PlayerRot, GetWorld()->GetDeltaSeconds(), 20.f));
	return EBTNodeResult::Succeeded;

	
	
}
