// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PoolingSubsystem.generated.h"


USTRUCT()
struct FPoolArray
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<AActor*> ObjectPool;

	bool IsEmpty()
	{
		return ObjectPool.IsEmpty();
	}

	AActor* Pop()
	{
		return ObjectPool.Pop();
	}

	void Add(AActor* ActorToAdd)
	{
		ObjectPool.Add(ActorToAdd);
	}
};

UCLASS()
class TEAMASSIGNMENTFPS_API UPoolingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Pool Subsystem")
	void SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation);

	UFUNCTION(BlueprintCallable, Category = "Pool Subsystem")
	void ReturnToPool(AActor* Poolable);

	UFUNCTION(BlueprintCallable, Category = "Pool Subsystem")
	void InitializePool(TSubclassOf<AActor> PoolClass, int32 MaxSize);
	
private:
	TMap<UClass*, FPoolArray> ObjectPools;

	AActor* GetActorFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation);
	
};
