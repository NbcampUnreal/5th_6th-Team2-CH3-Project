// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToCustom.generated.h"

struct FPathFollowingResult;

UCLASS()
class TEAMASSIGNMENTFPS_API UBTTask_MoveToCustom : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_MoveToCustom();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector EnemyAttackRange;
};
