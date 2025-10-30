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
	// ���ϴ� ���� ũ��(cm) ? ��: X=1000, Y=1000, Z=300
	UPROPERTY(EditAnywhere, Category = "NavMesh")
	FVector DesiredSize = FVector(1000.f, 1000.f, 300.f);
	// ������ NavMeshBoundsVolume ����
	UPROPERTY(VisibleAnywhere, Category = "NavMesh")
	ANavMeshBoundsVolume* SpawnedBounds = nullptr;
	// ���߿� ũ�⸦ �ٽ� �ٲٰ� ���� �� ȣ��
	UFUNCTION(BlueprintCallable, Category = "NavMesh")
	void ResizeNavBounds(ANavMeshBoundsVolume* Volume, const FVector& NewAbsoluteSize);
private:
	void RequestNavmeshRebuild();

};