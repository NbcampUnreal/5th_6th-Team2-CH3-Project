// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/AI/BTTask_EnemyMoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
//#include "GameFramework/Character.h"

UBTTask_EnemyMoveTo::UBTTask_EnemyMoveTo()
{
	EnemyAttackRange.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_EnemyMoveTo, EnemyAttackRange));
}

EBTNodeResult::Type UBTTask_EnemyMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float EnemyRange = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(EnemyAttackRange.SelectedKeyName);

	AcceptableRadius = EnemyRange;
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
