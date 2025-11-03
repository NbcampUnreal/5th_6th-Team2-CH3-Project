// Fill out your copyright notice in the Description page of Project Settings.


#include "Pooling/PoolingSubsystem.h"
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



void UPoolingSubsystem::SpawnFromPool(TSubclassOf<AActor> PoolClass, FVector Location, FRotator Rotation)
{
	GetActorFromPool(PoolClass, Location, Rotation);
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

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* NewActor = GetWorld()->SpawnActor<AActor>(PoolClass, Location, Rotation, SpawnParams);
	if (NewActor && PoolClass.Get()->ImplementsInterface((UPoolingInterface::StaticClass())))
	{
		IPoolingInterface::Execute_OnSpawnFromPool(NewActor);
	}
	return NewActor;

	
	// pool이 비어있을 경우 Pool 내부의 객체를 하나 지우고 이를 화면에 띄운다.
	// 비어있다면 객체를 새로 생성.
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
