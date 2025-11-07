#include "GameState/GameStateManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnemySpawner/EnemySpawner.h"
#include "EnemySpawner/EnemySpawnerManager.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "Controller/PlayerController/MyPlayerController.h"
#include "GameInstance/GameInstanceManager.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Trap/TrapBase.h"
#include "Debug/UELOGCategories.h"



AGameStateManager::AGameStateManager()
{
	Score = 0;
	LevelDuration = 5.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void AGameStateManager::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AGameStateManager::UPdateHUD,
		0.1f,
		true
	);
}

int32 AGameStateManager::GetScore() const
{
	return Score;
}

void AGameStateManager::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameInstanceManager* SpartaGameInstance = Cast<UGameInstanceManager>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void AGameStateManager::StartLevel()
{

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->ShowGameHUD();
		}
	}

	AEnemySpawnerManager* SpawnerManager = Cast<AEnemySpawnerManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnerManager::StaticClass())
	);
	
	if (!SpawnerManager)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnerManager not found"));
		return;
	}

	AEnemySpawner* EnemySpawner = SpawnerManager->LocateSpawnerAt(SpawnerManager->GetRandomSpawnLocation());
	
	const int32 MonsterToSpawn = 10;

	for (int32 i = 0; i < MonsterToSpawn; ++i)
	{
		AEnemyBaseCharacter* Spawned = EnemySpawner->SpawnRandomMonster(); // EnemySpawner �ʿ� ���� �ʿ�
		if (!Spawned)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%d] Monster spawn failed"), i);
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AGameStateManager::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void AGameStateManager::OnLevelTimeUp()
{	
	EndLevel();
}

void AGameStateManager::EndLevel()
{

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UGameInstanceManager* SpartaGameInstance = Cast<UGameInstanceManager>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentLevelIndex++;
			GetWorldTimerManager().ClearTimer(LevelTimerHandle);
			AddScore(Score);
			PhaseOver.Broadcast();
			FindTrap();

			AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

			// HUD가 켜져 있다면 닫기
			if (PlayerController)
			{
				PlayerController->HUDWidgetInstance->RemoveFromParent();
				PlayerController->HUDWidgetInstance = nullptr;
			}
;
			if (CurrentLevelIndex >= MaxLevels)
			{
				OnGameOver();
				return;
			}
		}
	}

}

void AGameStateManager::UPdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = MyPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Timer"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UGameInstanceManager* GameInstanceManager = Cast<UGameInstanceManager>(GameInstance);
						if (GameInstanceManager)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), GameInstanceManager->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}

void AGameStateManager::FindTrap()
{
	UWorld* World = GetWorld();
	if (!World) return;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ATrapBase::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		ATrapBase* Found = Cast<ATrapBase>(Actor);
		if (Found)
		{
			Found->TrapOn();
		}
	}
}

void AGameStateManager::NextLevel()
{
	AEnemySpawnerManager* SpawnerManager = Cast<AEnemySpawnerManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawnerManager::StaticClass())
	);

	if (!SpawnerManager)
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnerManager not found"));
		return;
	}

	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetWorld()->GetFirstPlayerController());

	// HUD가 켜져 있다면 닫기
	if (PlayerController)
	{
		PlayerController->ShowGameHUD();
	}

	AEnemySpawner* EnemySpawner = SpawnerManager->LocateSpawnerAt(SpawnerManager->GetRandomSpawnLocation());

	const int32 MonsterToSpawn = 10;

	for (int32 i = 0; i < MonsterToSpawn; ++i)
	{
		AEnemyBaseCharacter* Spawned = EnemySpawner->SpawnRandomMonster(); // EnemySpawner �ʿ� ���� �ʿ�
		if (!Spawned)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%d] Monster spawn failed"), i);
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AGameStateManager::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void AGameStateManager::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(PlayerController))
		{
			MyPlayerController->SetPause(true);
			MyPlayerController->ShowMainMenu(true);
		}
	}
}