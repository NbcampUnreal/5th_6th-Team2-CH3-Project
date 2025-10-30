#include "GameState/GameStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner/EnemySpawner.h"
#include "Debug/UELOGCategories.h"


AGameStateManager::AGameStateManager()
{
	Score = 0;
	LevelDuration = 30.0f; // 한 레벨당 30초
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AGameStateManager::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 첫 레벨부터 진행
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
	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
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
				//// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				//if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				//{
				//	SpawnedCoinCount++;
				//}
			}
		}
	}

	// 30초 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
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
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AGameStateManager::OnCoinCollected()
{
	//CollectedCoinCount++;

	//UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
	//	CollectedCoinCount,
	//	SpawnedCoinCount)

	//	// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
	//	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	//	{
	//		EndLevel();
	//	}
}

void AGameStateManager::EndLevel()
{
	// 타이머 해제
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// 다음 레벨 인덱스로
	CurrentLevelIndex++;

	// 모든 레벨을 다 돌았다면 게임 오버 처리
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

}

void AGameStateManager::OnGameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over!!"));
	// 여기서 UI를 띄운다거나, 재시작 기능을 넣을 수도 있음
}