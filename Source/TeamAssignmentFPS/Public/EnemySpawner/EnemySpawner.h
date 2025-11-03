#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "GameFramework/Actor.h"
#include "NavMesh/NavMeshBoundsVolume.h"     // ANavMeshBoundsVolume
#include "NavigationSystem.h"                // UNavigationSystemV1
#include "EnemySpawner.generated.h"

class AEnemyBaseCharacter;

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* MonsterDataTable = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void Init(AEnemySpawnerManager* InManager);

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AEnemyBaseCharacter* SpawnRandomMonster();

	FEnemyDataRow* GetRandomMonster() const;
	AEnemyBaseCharacter* SpawnMonster(TSubclassOf<AActor> MonsterClass);

	virtual void BeginPlay() override;

protected:

	UPROPERTY()
	AEnemySpawnerManager* Manager = nullptr;

};





