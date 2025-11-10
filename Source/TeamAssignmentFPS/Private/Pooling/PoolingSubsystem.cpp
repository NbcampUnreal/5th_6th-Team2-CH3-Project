// Fill out your copyright notice in the Description page of Project Settings.


#include "Pooling/PoolingSubsystem.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Pooling/PoolingInterface.h"
#include "Debug/UELOGCategories.h"

bool UPoolingSubsystem::InitializePool(TSubclassOf<AActor> PoolClass, int32 MaxSize)
{
	if (!PoolClass)
	{
		// error, invalid class
		return false;
	}
	
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);

	//Check if the world is valid
	if (!GetWorld())
	{
		//error, cannot get world
		return false;
	}
	
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
			
			//deactivate basics
			NewActor->SetActorHiddenInGame(true);
			NewActor->SetActorEnableCollision(false);
			NewActor->SetActorTickEnabled(false);
			
			ObjectPool.Add(NewActor);
		}
	}
	
	return true;
	//Pool 내부를 Maxsize만큼 반복해서 채우는 함수 
}



AActor* UPoolingSubsystem::BringFromPoolOrSpawn(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation)
{


	AActor* PooledActor=GetActorFromPool(PoolClass);

	if (PooledActor)// didnt found valid actor from the pool. just spawn the actor
	{
		//re activate the pooled actor
		PooledActor->SetActorLocationAndRotation(Location, Rotation);
		PooledActor->SetActorHiddenInGame(false);
		PooledActor->SetActorEnableCollision(true);
		PooledActor->SetActorTickEnabled(true);

		IPoolingInterface::Execute_OnSpawnFromPool(PooledActor);
		return PooledActor;
	}
	// if no valid actor from pool was found
	// spawn it
	UE_LOG(Pooling_Log, Warning, TEXT("UPoolingSubsystem::SpawnFromPool->Invalid Actor for PoolSystem, Try Spawning"));
	
	if (!GetWorld())
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::SpawnFromPool-> Failed to get World"));
		return nullptr;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewSpawnedActor = GetWorld()->SpawnActor<AActor>(PoolClass, Location, Rotation, SpawnParams);
	if (NewSpawnedActor)
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::SpawnFromPool-> Failed to spawn the actor"));
		return nullptr;
	}
	if (NewSpawnedActor && PoolClass.Get()->ImplementsInterface((UPoolingInterface::StaticClass())))
	{
		IPoolingInterface::Execute_OnSpawnFromPool(NewSpawnedActor);
	}
	return NewSpawnedActor;
}


AActor* UPoolingSubsystem::GetActorFromPool(TSubclassOf<AActor> PoolClass)
{

	if (!PoolClass)
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::GetActorFromPool-> Invalid PoolClass"));
		return nullptr;
	}
	if (!GetWorld())
	{
		//Error, cannot get world
		return nullptr;
	}
	
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(PoolClass);
	AActor* PooledActor=nullptr;//empty storage for aactor
	
	if (ObjectPool.IsEmpty())// if there is array for actor class
	{
		return nullptr;// no actor from the pool
	}
	
	AActor* Actor = ObjectPool.Pop();
	if (!Actor)
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::GetActorFromPool-> Actor from the Pool Array is nullptr"));
		// when the actor from the pool is invalid-> pooling management error
		return nullptr;
	}
	
	if (!Actor->GetClass()->ImplementsInterface(UPoolingInterface::StaticClass()))// if pooled actor has no pool interface
	{
		UE_LOG(Pooling_Log, Error, TEXT("UPoolingSubsystem::GetActorFromPool-> Actor from the Pool Array has no pool interface"));
		return nullptr;
	}

	IPoolingInterface::Execute_OnSpawnFromPool(Actor);
	return Actor;
}


bool UPoolingSubsystem::ReturnToPoolOrDestroy(AActor* Poolable, bool & bWasDestroyed)
{
	if (!Poolable)
	{
		//Error, invalid Actor
		bWasDestroyed=false;
		return false;
	}

	UClass* ActorClass = Poolable->GetClass();

	if (!ActorClass->ImplementsInterface(UPoolingInterface::StaticClass()))// if the actor doesnt have interface
	{
		Poolable->Destroy();
		//Warning, Actors does not have pooling interface. just delete
		bWasDestroyed=true;
		return false;
	}

	// if the actor has the interface
	IPoolingInterface::Execute_OnReturnToPool(Poolable);
		
	Poolable->SetActorHiddenInGame(true);
	Poolable->SetActorEnableCollision(false);
	Poolable->SetActorTickEnabled(false);
		
	FPoolArray& ObjectPool = ObjectPools.FindOrAdd(ActorClass); // add to preexisting, or add new pool array/
	ObjectPool.Add(Poolable);
	
	bWasDestroyed=false;
	return true;
	
}
