#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameStateManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPhaseOver);

UCLASS()
class TEAMASSIGNMENTFPS_API AGameStateManager : public AGameState
{
	

	GENERATED_BODY()

public:
	AGameStateManager();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite, Category = "Score")
	int32 Score;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(BlueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	// 게임이 완전히 끝났을 때 (모든 레벨 종료) 실행되는 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	// 레벨을 시작할 때, 아이템 스폰 및 타이머 설정
	void StartLevel();
	// 레벨 제한 시간이 만료되었을 때 호출
	void OnLevelTimeUp();
	// 코인을 주웠을 때 호출
	void OnCoinCollected();
	// 레벨을 강제 종료하고 다음 레벨로 이동
	void EndLevel();
	void UPdateHUD();
	// 오브젝트 상호작용
	void FindObject();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void NextLevel();

	UFUNCTION(BlueprintCallable, Category = "Phase")
	void NextPhase();

	UPROPERTY()
	FPhaseOver PhaseOver;
};





