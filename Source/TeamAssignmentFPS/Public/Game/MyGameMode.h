// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AMyGameMode();

protected:

	virtual void BeginPlay() override;	
	
private:
	UPROPERTY(EditAnywhere, Category= "Pooling")
	TSubclassOf<AActor> MeleeEnemyClass;

	UPROPERTY(EditAnywhere, Category= "Pooling")
	TSubclassOf<AActor> RangeEnemyClass;
	
	UPROPERTY(EditAnywhere, Category= "Pooling")
	TSubclassOf<AActor> PlayerProjectileClass;

	UPROPERTY(EditAnywhere, Category= "Pooling")
	TSubclassOf<AActor> EnemyProjectileClass;

	void InitializePooling() const;
	
};
