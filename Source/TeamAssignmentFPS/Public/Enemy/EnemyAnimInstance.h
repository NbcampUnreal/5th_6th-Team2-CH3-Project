// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/EnemyState/EnemyTypes.h"
#include "EnemyAnimInstance.generated.h"

class AEnemyBaseCharacter;

UCLASS()
class TEAMASSIGNMENTFPS_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	AEnemyBaseCharacter* Enemy;
	
	 virtual void NativeInitializeAnimation() override;
	 virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
public:
	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState;

	UPROPERTY(BlueprintReadOnly)
	float Speed;
private:
	UFUNCTION()
	void AnimNotify_SpawnEnd();
};
