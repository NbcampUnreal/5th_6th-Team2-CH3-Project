// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BTTask_ClearInvestigating.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ClearInvestigating::UBTTask_ClearInvestigating()
{
	NodeName = TEXT("Clear Investigating");
}

EBTNodeResult::Type UBTTask_ClearInvestigating::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent BlackboardComp = OwnerComp->

	return EBTNodeResult::Type();
}
