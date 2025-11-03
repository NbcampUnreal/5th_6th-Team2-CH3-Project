#include "EnemySpawner/EnemySpawnerManager.h"
#include "Components/BoxComponent.h"
#include "EnemySpawner/EnemySpawner.h"
#include "Enemy/EnemyBaseCharacter.h"

AEnemySpawnerManager::AEnemySpawnerManager()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	SpawnerClass = AEnemySpawner::StaticClass();

}

AEnemySpawner* AEnemySpawnerManager::LocateSpawnerAt(const FVector& Location)
{

	FVector SpawnLocation = GetRandomSpawnLocation();

	AEnemySpawner* SpawnActor = GetWorld()->SpawnActor<AEnemySpawner>(
		SpawnerClass,
		SpawnLocation,
		FRotator::ZeroRotator
	);

	if (SpawnActor)
	{
		SpawnActor->Init(this);
	}

	return SpawnActor;

}

void AEnemySpawnerManager::AddSpawnLocation(const FVector& SpawnLocation)
{
	SpawnLocations.Add(SpawnLocation);
}

// 마지막 위치 가져오기
FVector AEnemySpawnerManager::GetLastSpawnLocation() const
{
	if (SpawnLocations.Num() > 0)
		return SpawnLocations.Last();
	return FVector::ZeroVector;
}

// 모든 위치 리스트 가져오기
const TArray<FVector>& AEnemySpawnerManager::GetSpawnLocations() const
{
	return SpawnLocations;
}

// 위치를 랜덤으로 구하는 함수
FVector AEnemySpawnerManager::GetRandomSpawnLocation()
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	FVector Result = BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		0.f
	);

	AddSpawnLocation(Result);

	return Result;
}


//FVector AEnemySpawnerManager::GetRandomPointInVolume() const
//{
//	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
//	FVector BoxOrigin = SpawningBox->GetComponentLocation();
//
//	return BoxOrigin + FVector(
//		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
//		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
//		0
//	);
//}




