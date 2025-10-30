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
	// 1) NavMeshBoundsVolume 스폰
	SpawnedBounds = World->SpawnActor<ANavMeshBoundsVolume>(
		ANavMeshBoundsVolume::StaticClass(),
		SpawnLocation,
		SpawnRotation,
		Params
	);
	if (!SpawnedBounds) return;
	// 2) 절대 크기에 맞춰 스케일 계산/적용
	ResizeNavBounds(SpawnedBounds, DesiredSize);
	// 3) 내비게이션 갱신 요청 (Runtime Generation이 Dynamic이면 보통 자동이나, 안전하게 한 번 알림)
	RequestNavmeshRebuild();

}

void ANavSpawner::ResizeNavBounds(ANavMeshBoundsVolume* Volume, const FVector& NewAbsoluteSize)
{
	if (!Volume) return;
	// 스케일을 1로 초기화 후 현재 바운드 크기를 측정
	Volume->SetActorScale3D(FVector(1.f, 1.f, 1.f));
	// 현재 컴포넌트들의 바운드 박스(월드 기준) 크기
	const FBox Bounds = Volume->GetComponentsBoundingBox(true);
	const FVector CurrentSize = Bounds.GetSize(); // (X,Y,Z) in cm
	// 0으로 나눔 방지
	FVector SafeCurrentSize = CurrentSize;
	if (SafeCurrentSize.X <= KINDA_SMALL_NUMBER) SafeCurrentSize.X = 1.f;
	if (SafeCurrentSize.Y <= KINDA_SMALL_NUMBER) SafeCurrentSize.Y = 1.f;
	if (SafeCurrentSize.Z <= KINDA_SMALL_NUMBER) SafeCurrentSize.Z = 1.f;
	// 원하는 절대 크기 / 현재 크기 = 스케일 비율
	const FVector Scale(
		NewAbsoluteSize.X / SafeCurrentSize.X,
		NewAbsoluteSize.Y / SafeCurrentSize.Y,
		NewAbsoluteSize.Z / SafeCurrentSize.Z
	);
	Volume->SetActorScale3D(Scale);
}

void ANavSpawner::RequestNavmeshRebuild()
{
	// 내비게이션 시스템 가져오기
	if (UWorld* World = GetWorld())
	{
		if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World))
		{
			// 갱신 알림: 스폰/크기 변경된 Bounds 정보를 NavSystem에 전달
			if (SpawnedBounds)
			{
				// 특정 Bounds가 바뀌었음을 알림 (엔진 버전에 따라 함수명/노출이 약간 다를 수 있음)
				NavSys->OnNavigationBoundsUpdated(SpawnedBounds);
			}
			// 필요 시 전체 리빌드(프로젝트 설정에 따라 자동 갱신이면 생략 가능)
			// NavSys->Build();
		}
	}
}


