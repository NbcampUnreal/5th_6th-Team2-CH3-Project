// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetourCrowdAIController.h"
#include "Enemy/EnemyState/EnemyTypes.h"
#include "EnemyDetourCrowdAIController.generated.h"

/**
 * 
 */
// Fill out your copyright notice in the Description page of Project Settings.


UCLASS()
class TEAMASSIGNMENTFPS_API AEnemyDetourCrowdAIController : public ADetourCrowdAIController
{
	GENERATED_BODY()
	
public:
	AEnemyDetourCrowdAIController();

protected:

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UBlackboardComponent> BlackboardComp;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	void DefaultSettingBlackBoard();

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:
	void StartBehaviorTree();
	void StopBehaviorTree();

public:
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

};

