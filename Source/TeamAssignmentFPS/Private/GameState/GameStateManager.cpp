#include "GameState/GameStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner/EnemySpawner.h"
#include "Debug/UELOGCategories.h"


AGameStateManager::AGameStateManager()
{
	Score = 0;
	LevelDuration = 30.0f; // �� ������ 30��
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AGameStateManager::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ù �������� ����
	StartLevel();
}

int32 AGameStateManager::GetScore() const
{
	return Score;
}

void AGameStateManager::AddScore(int32 Amount)
{
	Score += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}

void AGameStateManager::StartLevel()
{
	// ���� �ʿ� ��ġ�� ��� SpawnVolume�� ã�� ������ 40���� ����
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundVolumes);
	UE_LOG(GameState_Log, Warning, TEXT("StartLevel"))

	const int32 ItemToSpawn = 40;

	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(FoundVolumes[0]);
			if (EnemySpawner)
			{
				AActor* SpawnedActor = EnemySpawner->SpawnRandomMonster();
				//// ���� ������ ���Ͱ� ���� Ÿ���̶�� SpawnedCoinCount ����
				//if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				//{
				//	SpawnedCoinCount++;
				//}
			}
		}
	}

	// 30�� �Ŀ� OnLevelTimeUp()�� ȣ��ǵ��� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AGameStateManager::OnLevelTimeUp,
		LevelDuration,
		false
	);

	//UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
	//	CurrentLevelIndex + 1,
	//	SpawnedCoinCount);
}

void AGameStateManager::OnLevelTimeUp()
{
	// �ð��� �� �Ǹ� ������ ����
	EndLevel();
}

void AGameStateManager::OnCoinCollected()
{
	//CollectedCoinCount++;

	//UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
	//	CollectedCoinCount,
	//	SpawnedCoinCount)

	//	// ���� �������� ������ ������ ���� �ֿ��ٸ� ��� ���� ����
	//	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	//	{
	//		EndLevel();
	//	}
}

void AGameStateManager::EndLevel()
{
	// Ÿ�̸� ����
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// ���� ���� �ε�����
	CurrentLevelIndex++;

	// ��� ������ �� ���Ҵٸ� ���� ���� ó��
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

}

void AGameStateManager::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
	// ���⼭ UI�� ���ٰų�, ����� ����� ���� ���� ����
}