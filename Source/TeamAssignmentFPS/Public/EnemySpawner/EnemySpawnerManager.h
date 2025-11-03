#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnerManager.generated.h"

class AEnemySpawner;
class UBoxComponent;
class AEnemyBaseCharacter;

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemySpawnerManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnerManager();
	
	TArray<AEnemySpawner*> EnemySpawners;

	AEnemySpawner* LocateSpawnerAt(const FVector& Location);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	//FVector GetRandomPointInVolume() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<AEnemySpawner> SpawnerClass;

private:
	// 스폰 위치 저장용 리스트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<FVector> SpawnLocations;

public:
	UFUNCTION(BlueprintCallable)
	void AddSpawnLocation (const FVector& SpawnLocation);

	// 마지막 위치 가져오기
	UFUNCTION(BlueprintCallable)
	FVector GetLastSpawnLocation() const;

	// 모든 위치 리스트 가져오기
	UFUNCTION(BlueprintCallable)
	const TArray<FVector>& GetSpawnLocations() const;

	// 위치를 랜덤으로 구하는 함수
	FVector GetRandomSpawnLocation();
};
