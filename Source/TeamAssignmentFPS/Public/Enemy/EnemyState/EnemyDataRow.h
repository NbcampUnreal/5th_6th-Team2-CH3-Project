// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyState/EnemyTypes.h"
#include "EnemyDataRow.generated.h"


USTRUCT(BlueprintType)
struct FEnemyDataRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Enemy")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 MaxHP;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float HeightMinRatio;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float HeightMaxRatio;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 Damage;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float Range;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float Delay;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 Score;
};

