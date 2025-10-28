#include "EnemySpawner/EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawingBox"));
	SpawningBox->SetupAttachment(Scene);

	MonsterDataTable = nullptr;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	if (TriggerBox != nullptr)
		return;

	RootComponent = TriggerBox;
}


void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::OnOverlapBegin);

	DrawDebugSphere(GetWorld(), SpawnLocation, SpawnRadius, 12, FColor::White, true);
}

void AEnemySpawner::SpawnRandomMonster()
{
	
}

FVector AEnemySpawner::GetRandomPontInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();
	
	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

void AEnemySpawner::SpawnMonster(TSubclassOf<AActor> MonsterClass)
{
	if (!MonsterClass)
		return;

	GetWorld()->SpawnActor<AActor>(
		MonsterClass,
		GetRandomPontInVolume(),
		FRotator::ZeroRotator
	);
}

void AEnemySpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		for (int i = 0; i < NumberOfSpawn; i++)
		{
			FNavLocation EnemySpawnLocation;
			UNavigationSystemV1::GetNavigationSystem(GetWorld())->GetRandomReachablePointInRadius(SpawnLocation, SpawnRadius, EnemySpawnLocation);

			auto SpawnedEnemy = GetWorld()->SpawnActor<ACharacter>(EnemyBpRef, UKismetMathLibrary::MakeTransform(EnemySpawnLocation, UKismetMathLibrary::FindLookAtRotation(EnemySpawnLocation, GetActorLocation())));

			if (SpawnedEnemy)
			{
				//FRotator NewRotation = SpawnedEnemy->GetActorRotation();
				//NewRotation.Pitch = 0;

				//SpawnedEnemy->SetActorRotation(NewRotation);
				//SpawnedEnemy->SpawnDefaultController();
			}
		}
	}

	Destroy();

}



