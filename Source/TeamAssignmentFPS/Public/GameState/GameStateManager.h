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
    // ?????? ?????? ?????? ?? (??? ???? ????) ?????? ???
    UFUNCTION(BlueprintCallable, Category = "Level")
    void OnGameOver();

    // ?????? ?????? ??, ?????? ???? ?? ???? ????
    void StartLevel();
    // ???? ???? ?©£??? ???????? ?? ???
    void OnLevelTimeUp();
    // ?????? ????? ?? ???
    void OnCoinCollected();
    // ?????? ???? ??????? ???? ?????? ???
    void EndLevel();
    void UPdateHUD();
    void FindObject();
    void NextLevel();

    UFUNCTION(BlueprintCallable, Category = "Phase")
    void NextPhase();

    UPROPERTY()
    FPhaseOver PhaseOver;
};