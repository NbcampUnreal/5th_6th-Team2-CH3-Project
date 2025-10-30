#include "NavSpawner/NavSpawner.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshBoundsVolume.h"

ANavSpawner::ANavSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ANavSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (!World) return;
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	const FVector SpawnLocation = GetActorLocation();
	const FRotator SpawnRotation = FRotator::ZeroRotator;
	// 1) NavMeshBoundsVolume ����
	SpawnedBounds = World->SpawnActor<ANavMeshBoundsVolume>(
		ANavMeshBoundsVolume::StaticClass(),
		SpawnLocation,
		SpawnRotation,
		Params
	);
	if (!SpawnedBounds) return;
	// 2) ���� ũ�⿡ ���� ������ ���/����
	ResizeNavBounds(SpawnedBounds, DesiredSize);
	// 3) ������̼� ���� ��û (Runtime Generation�� Dynamic�̸� ���� �ڵ��̳�, �����ϰ� �� �� �˸�)
	RequestNavmeshRebuild();

}

void ANavSpawner::ResizeNavBounds(ANavMeshBoundsVolume* Volume, const FVector& NewAbsoluteSize)
{
	if (!Volume) return;
	// �������� 1�� �ʱ�ȭ �� ���� �ٿ�� ũ�⸦ ����
	Volume->SetActorScale3D(FVector(1.f, 1.f, 1.f));
	// ���� ������Ʈ���� �ٿ�� �ڽ�(���� ����) ũ��
	const FBox Bounds = Volume->GetComponentsBoundingBox(true);
	const FVector CurrentSize = Bounds.GetSize(); // (X,Y,Z) in cm
	// 0���� ���� ����
	FVector SafeCurrentSize = CurrentSize;
	if (SafeCurrentSize.X <= KINDA_SMALL_NUMBER) SafeCurrentSize.X = 1.f;
	if (SafeCurrentSize.Y <= KINDA_SMALL_NUMBER) SafeCurrentSize.Y = 1.f;
	if (SafeCurrentSize.Z <= KINDA_SMALL_NUMBER) SafeCurrentSize.Z = 1.f;
	// ���ϴ� ���� ũ�� / ���� ũ�� = ������ ����
	const FVector Scale(
		NewAbsoluteSize.X / SafeCurrentSize.X,
		NewAbsoluteSize.Y / SafeCurrentSize.Y,
		NewAbsoluteSize.Z / SafeCurrentSize.Z
	);
	Volume->SetActorScale3D(Scale);
}

void ANavSpawner::RequestNavmeshRebuild()
{
	// ������̼� �ý��� ��������
	if (UWorld* World = GetWorld())
	{
		if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
		{
			// ���� �˸�: ����/ũ�� ����� Bounds ������ NavSystem�� ����
			if (SpawnedBounds)
			{
				// Ư�� Bounds�� �ٲ������ �˸� (���� ������ ���� �Լ���/������ �ణ �ٸ� �� ����)
				NavSys->OnNavigationBoundsUpdated(SpawnedBounds);
			}
			// �ʿ� �� ��ü ������(������Ʈ ������ ���� �ڵ� �����̸� ���� ����)
			// NavSys->Build();
		}
	}
}


