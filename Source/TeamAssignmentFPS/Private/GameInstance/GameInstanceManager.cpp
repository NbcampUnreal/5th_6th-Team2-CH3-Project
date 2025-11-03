#include "GameInstance/GameInstanceManager.h"

UGameInstanceManager::UGameInstanceManager()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UGameInstanceManager::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}
