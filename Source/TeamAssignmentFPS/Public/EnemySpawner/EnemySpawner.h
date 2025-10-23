#pragma once

#include "CoreMinimal.h"
//#include "MonsterSpawnRow.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBoxComponent;
class FMonsterSpawnRow;

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
	void SpawnRandomMonster();

	//FMonsterSpawnRow* GetRandomMonster() const;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPontInVolume() const;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnMonster(TSubclassOf<AActor> MonsterClass);

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACharacter> EnemyBpRef;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	int32 NumberOfSpawn;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float SpawnRadius;
};
