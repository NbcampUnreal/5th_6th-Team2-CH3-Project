#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "GameFramework/Actor.h"
#include "NavMesh/NavMeshBoundsVolume.h"     // ANavMeshBoundsVolume
#include "NavigationSystem.h"                // UNavigationSystemV1
#include "EnemySpawner.generated.h"

class UBoxComponent;
class ANavMeshBoundsVolume;

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* MonsterDataTable;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomMonster();

	FEnemyDataRow* GetRandomMonster() const;
	AActor* SpawnMonster(TSubclassOf<AActor> MonsterClass);
	FVector GetRandomPontInVolume() const;

	virtual void BeginPlay() override;
};





