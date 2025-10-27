// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTask_MoveToCustom.h"
//#include "AIController.h"
#include "Enemy/EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/Character.h"


UBTTask_MoveToCustom::UBTTask_MoveToCustom()
{
	NodeName = TEXT("Move To Custom");

	EnemyAttackRange.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToCustom, EnemyAttackRange));

}

EBTNodeResult::Type UBTTask_MoveToCustom::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//AAIController* AIController = OwnerComp.GetAIOwner();
	
	AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
		
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey());
	AActor* TargetActor = Cast<AActor>(TargetObject);
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}
		
	float EnemyRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(EnemyAttackRange.SelectedKeyName);


	FAIMoveRequest MoveReq;
	MoveReq.SetGoalActor(TargetActor);
	MoveReq.SetAcceptanceRadius(EnemyRange);

	FNavPathSharedPtr NavPath;
	AIController->MoveTo(MoveReq, &NavPath);

	return EBTNodeResult::Aborted;
}

