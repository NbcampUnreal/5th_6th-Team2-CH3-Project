#include "EnemySpawner/EnemySpawner.h"
#include "Components/BoxComponent.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawingBox"));
	SpawningBox->SetupAttachment(Scene);
}

FVector AEnemySpawner::GetRandomPontInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

void AEnemySpawner::SpawnMonster(TSubclassOf<AActor> MonsterClass)
{
	if (!MonsterClass)
		return;

	GetWorld()->SpawnActor<AActor>(
		MonsterClass,
		GetRandomPontInVolume(),
		FRotator::ZeroRotator
	);
}


