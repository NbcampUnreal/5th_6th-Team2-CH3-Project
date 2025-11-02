// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPoolingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAMASSIGNMENTFPS_API IPoolingInterface
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnSpawnFromPool();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Pooling")
	void OnReturnToPool();
	
};
