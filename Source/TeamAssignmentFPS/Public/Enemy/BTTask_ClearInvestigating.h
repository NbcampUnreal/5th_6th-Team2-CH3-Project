// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ClearInvestigating.generated.h"

/**
 * 
 */
UCLASS()
class TEAMASSIGNMENTFPS_API UBTTask_ClearInvestigating : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ClearInvestigating();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
