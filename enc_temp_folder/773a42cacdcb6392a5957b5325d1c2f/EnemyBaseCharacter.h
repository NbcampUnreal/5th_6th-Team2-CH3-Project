// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "EnemyBaseCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnEnemyDead, int Score);

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();

	FOnEnemyDead OnEnemyDead;
protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnmeyStats;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyType EnemyType;

};

