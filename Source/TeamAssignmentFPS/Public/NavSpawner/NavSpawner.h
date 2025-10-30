#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NavSpawner.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API ANavSpawner : public AActor
{
	GENERATED_BODY()

public:
	ANavSpawner();

protected:
	virtual void BeginPlay() override;

public:
	// 원하는 절대 크기(cm) ? 예: X=1000, Y=1000, Z=300
	UPROPERTY(EditAnywhere, Category = "NavMesh")
	FVector DesiredSize = FVector(1000.f, 1000.f, 300.f);
	// 스폰된 NavMeshBoundsVolume 참조
	UPROPERTY(VisibleAnywhere, Category = "NavMesh")
	ANavMeshBoundsVolume* SpawnedBounds = nullptr;
	// 나중에 크기를 다시 바꾸고 싶을 때 호출
	UFUNCTION(BlueprintCallable, Category = "NavMesh")
	void ResizeNavBounds(ANavMeshBoundsVolume* Volume, const FVector& NewAbsoluteSize);
private:
	void RequestNavmeshRebuild();

};