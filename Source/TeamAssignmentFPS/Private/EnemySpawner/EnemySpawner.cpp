#include "EnemySpawner/EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawingBox"));
	SpawningBox->SetupAttachment(Scene);

	MonsterDataTable = nullptr;
}


void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

AActor* AEnemySpawner::SpawnRandomMonster()
{
<<<<<<< HEAD
	if (FEnemyDataRow* SelectedRow = GetRandomMonster())
	{
		if (UClass* ActualClass = SelectedRow->EnemyClass.Get())
		{
			return SpawnMonster(ActualClass);
		}
	}
	return nullptr;
}

FEnemyDataRow* AEnemySpawner::GetRandomMonster() const
{
	if (!MonsterDataTable) return nullptr;

	TArray<FEnemyDataRow*> AllRows;
	static const FString ContextString(TEXT("MonsterSpawnContext"));
	MonsterDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty())
	{
		return nullptr;
	}

	float TotalChance = 0.0f;

	for (const FEnemyDataRow* Row : AllRows)
	{
		if (Row)
		{
			TotalChance += Row->SpawnChance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);

	float AccumulateChance = 0.0f;

	for (FEnemyDataRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

=======
	
}

>>>>>>> 1352d0afd57722be3c883d8d9723dc97ae6f0461
FVector AEnemySpawner::GetRandomPontInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		0
	);
}

AActor* AEnemySpawner::SpawnMonster(TSubclassOf<AActor> MonsterClass)
{
	if (!MonsterClass)
		return nullptr;

	AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(
		MonsterClass,
		GetRandomPontInVolume(),
		FRotator::ZeroRotator
	);

	return SpawnActor;
}