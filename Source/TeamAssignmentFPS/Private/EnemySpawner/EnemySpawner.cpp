#include "EnemySpawner/EnemySpawner.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "EnemySpawner/EnemySpawnerManager.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	MonsterDataTable = nullptr;
}


void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();


}

void AEnemySpawner::Init(AEnemySpawnerManager* InManager)
{
	Manager = InManager;
}

AEnemyBaseCharacter* AEnemySpawner::SpawnRandomMonster()
{
	if (!MonsterDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: MonsterDataTable is null."));
		return nullptr;
	}
	if (!Manager)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: Manager is null. (Init not called?)"));
		return nullptr;
	}

	FEnemyDataRow* Row = GetRandomMonster();
	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawner: GetRandomMonster returned null."));
		return nullptr;
	}

	if (UClass* ActualClass = Row->EnemyClass.Get())
	{
		return SpawnMonster(ActualClass);
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

AEnemyBaseCharacter* AEnemySpawner::SpawnMonster(TSubclassOf<AActor> MonsterClass)
{
	if (!GetWorld() || !Manager || !*MonsterClass)
		return nullptr;
	if (!MonsterClass->IsChildOf(AEnemyBaseCharacter::StaticClass()))
		return nullptr;
	const FVector SpawnLocation = Manager->GetRandomSpawnLocation(); // ¹Ú½º ³»ºÎ ·£´ý ÁÂÇ¥
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	return GetWorld()->SpawnActor<AEnemyBaseCharacter>(
		(TSubclassOf<AEnemyBaseCharacter>)MonsterClass,
		SpawnLocation,
		FRotator::ZeroRotator,
		Params
	);
}