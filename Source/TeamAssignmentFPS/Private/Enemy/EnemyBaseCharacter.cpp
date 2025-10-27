
#include "Enemy/EnemyBaseCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "Debug/UELOGCategories.h"
#include "CharacterStat/HealthComponent.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	//PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));


	UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bOrientRotationToMovement = true;

	Movement->MaxWalkSpeed = 250.f;

	EnemyData.Range = 200.f;
}


void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDeath.AddUObject(this, &AEnemyBaseCharacter::EnemyDead);
}

void AEnemyBaseCharacter::EnemyAttack()
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack"));

	ChangeEnemyState(EEnemyState::EES_Attack);

	FTimerHandle TestTimerHandle;

	GetWorldTimerManager().SetTimer(TestTimerHandle, this, &AEnemyBaseCharacter::EnemyAttackEnd, 3.f);

}

void AEnemyBaseCharacter::EnemyAttackEnd()
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack End"));

	ChangeEnemyState(EEnemyState::EES_Chase);
}

void AEnemyBaseCharacter::TakeDamage(FDamageInfo)
{
	//데미지 받을 때 호출할 함수

	if (GetEnemyState() == EEnemyState::EES_Spawn)
	{
		return;
	}

	ChangeEnemyState(EEnemyState::EES_Damaged);
}

void AEnemyBaseCharacter::InitializeEnemyData(FEnemyDataRow& InData)
{
	EnemyData.EnemyType = InData.EnemyType;
	HealthComponent->SetMaxHealth(InData.MaxHP);
	HealthComponent->SetCurrentHealth(HealthComponent->GetMaxHealth());
	EnemyData.MoveSpeed = InData.MoveSpeed;
	EnemyData.HeightMinRatio = InData.HeightMinRatio;
	EnemyData.HeightMaxRatio = InData.HeightMaxRatio;
	EnemyData.Damage = InData.Damage;
	EnemyData.Range = InData.Range;
	EnemyData.Delay = InData.Delay;
	EnemyData.Score = InData.Score;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bOrientRotationToMovement = true;

	Movement->MaxWalkSpeed = EnemyData.MoveSpeed;
}

void AEnemyBaseCharacter::EnemyDead()
{
	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	ChangeEnemyState(EEnemyState::EES_Dead);
}

void AEnemyBaseCharacter::ChangeEnemyState(EEnemyState NewEnemyState)
{
	EnemyState = NewEnemyState;

	OnEnemyStateChanged.ExecuteIfBound(EnemyState);
}