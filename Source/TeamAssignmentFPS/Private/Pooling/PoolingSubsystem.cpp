// Fill out your copyright notice in the Description page of Project Settings.


#include "Pooling/PoolingSubsystem.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Pooling/PoolingInterface.h"

void UPoolingSubsystem::InitializePool(TSubclassOf<AActor> PoolClass, int32 MaxSize)
{
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
	for (int32 i = 0; i < MaxSize; i++)
	{
		FActorSpawnParameters SpawnParams;
		//충돌 여부와 상관없이 액터 항상 스폰
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AActor* NewActor = GetWorld()->SpawnActor<AActor>
		(PoolClass,
		FVector(0.f, 0.f, -100.f),
		FRotator::ZeroRotator,
		SpawnParams);
		
		if (NewActor && PoolClass.Get()->ImplementsInterface(UPoolingInterface::StaticClass()))
		{
			IPoolingInterface::Execute_OnReturnToPool(NewActor);
			NewActor->SetActorHiddenInGame(true);
			NewActor->SetActorEnableCollision(false);
			ObjectPool.Add(NewActor);
		}
	}
	
	//Pool 내부를 Maxsize만큼 반복해서 채우는 함수 
}



AActor* UPoolingSubsystem::SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation)
{
	return GetActorFromPool(PoolClass, Location, Rotation);
}


AActor* UPoolingSubsystem::GetActorFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation)
{
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
	if (!ObjectPool.IsEmpty())
	{
		AActor* Actor = ObjectPool.Pop();
		if (Actor)
		{
			Actor->SetActorLocationAndRotation(Location, Rotation);
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			
			IPoolingInterface::Execute_OnSpawnFromPool(Actor);
			return Actor;
		}
	}

<<<<<<< HEAD
		IPoolingInterface::Execute_OnSpawnFromPool(PooledActor);
		return PooledActor;
	}
	// if no valid actor from pool was found
	// spawn it
	UE_LOG(Pooling_Log, Warning, TEXT("UPoolingSubsystem::BringFromPoolOrSpawn->Invalid Actor for PoolSystem, Try Spawning"));
	
	if (!GetWorld())
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::BringFromPoolOrSpawn-> Failed to get World"));
		return nullptr;
	}
	
=======
>>>>>>> cccbfc2 (Revert "murge into seo")
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(PoolClass, Location, Rotation, SpawnParams);
	if (NewActor && PoolClass.Get()->ImplementsInterface((UPoolingInterface::StaticClass())))
	{
		IPoolingInterface::Execute_OnSpawnFromPool(NewActor);
	}
	return NewActor;

<<<<<<< HEAD
	AActor* NewSpawnedActor = GetWorld()->SpawnActor<AActor>(PoolClass, Location, Rotation, SpawnParams);
	if (!NewSpawnedActor)// when even the spawned actor is invalid-> pooling, spawning failed
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::BringFromPoolOrSpawn-> Failed to spawn the actor"));
		return nullptr;
	}
	if (PoolClass.Get()->ImplementsInterface((UPoolingInterface::StaticClass())))
	{
		IPoolingInterface::Execute_OnSpawnFromPool(NewSpawnedActor);
	}
	return NewSpawnedActor;
=======
	
	// pool이 비어있을 경우 Pool 내부의 객체를 하나 지우고 이를 화면에 띄운다.
	// 비어있다면 객체를 새로 생성.
>>>>>>> cccbfc2 (Revert "murge into seo")
}


void UPoolingSubsystem::ReturnToPool(AActor* Poolable)
{
	if (!Poolable)
	{
		return;
	}

	UClass* ActorClass = Poolable->GetClass();

	if (ActorClass->ImplementsInterface(UPoolingInterface::StaticClass()))
	{
		IPoolingInterface::Execute_OnReturnToPool(Poolable);
		Poolable->SetActorHiddenInGame(true);
		Poolable->SetActorEnableCollision(false);
		FPoolArray& ObjectPool = ObjectPools.FindOrAdd(ActorClass);
		ObjectPool.Add(Poolable);
	}
	else
	{
		Poolable->Destroy();
	}
	
}
