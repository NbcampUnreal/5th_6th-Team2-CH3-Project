// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawn.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemySpawn : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawn();

protected:
	virtual void BeginPlay() override;

private:


};
