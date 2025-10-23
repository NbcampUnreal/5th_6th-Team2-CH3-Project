// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "RangeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TEAMASSIGNMENTFPS_API ARangeEnemyCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()

public:
	ARangeEnemyCharacter();

protected:
	virtual void LoadEnemyDataRow() override;

};
