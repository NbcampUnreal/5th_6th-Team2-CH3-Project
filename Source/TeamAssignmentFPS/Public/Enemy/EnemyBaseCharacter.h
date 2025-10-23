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
	AEnemyBaseCharacter();

	FOnEnemyDead OnEnemyDead;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TObjectPtr<UDataTable> EnemyDataTable;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnemyState;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyType EnemyType;

	FEnemyDataRow* EnemyData;

	virtual void LoadEnemyDataRow();

	virtual void Attack() PURE_VIRTUAL(AEnemyBaseCharacter::Attack,);

public:

	FORCEINLINE float GetEnemyAttackRange() const { return EnemyData->Range; }

};

