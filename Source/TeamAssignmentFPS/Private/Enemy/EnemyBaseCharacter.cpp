#include "Enemy/EnemyBaseCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "CharacterStat/HealthComponent.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;

	Movement->MaxWalkSpeed = 250.f;

	EnemyData.Range = 200.f;
	EnemyData.Damage = 50;
}

AEnemyBaseCharacter::AEnemyBaseCharacter(FEnemyDataRow& InData)
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);

	//PrimaryActorTick.bCanEverTick = false;
	InitializeEnemyData(InData);
}


void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDeath.AddUObject(this, &AEnemyBaseCharacter::EnemyDead);
	HealthComponent->OnDamage.BindUObject(this, &AEnemyBaseCharacter::EnemyTakeDamage);
	//Enemy->OnEnemyDead.BindUObject(this, &GamestateManager::AddScore);

	

	HealthComponent->SetMaxHealth(100);
	HealthComponent->SetCurrentHealth(100);
}

void AEnemyBaseCharacter::EnemyAttack()
{
	//UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack"));

	if (EnemyState == EEnemyState::EES_Dead)
	{
		return;
	}

	ChangeEnemyState(EEnemyState::EES_Attack);

	FTimerHandle TestTimerHandle;

	GetWorldTimerManager().SetTimer(TestTimerHandle, this, &AEnemyBaseCharacter::EnemyAttackEnd, 3.f);

}

void AEnemyBaseCharacter::EnemyAttackEnd()
{
	//UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack End"));

	ChangeEnemyState(EEnemyState::EES_Chase);
}

void AEnemyBaseCharacter::EnemyTakeDamage(FDamageInfo)
{
	//데미지 받을 때 호출할 함수

	UE_LOG(Enemy_Log, Error, TEXT("Enemy Damaged"));

	if (GetEnemyState() == EEnemyState::EES_Spawn)
	{
		return;
	}

	ChangeEnemyState(EEnemyState::EES_Damaged);

	
}

void AEnemyBaseCharacter::EnemyDead()
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Dead"));

	SetEnemyNoCollision();
	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	ChangeEnemyState(EEnemyState::EES_Dead);
	
	Destroy();
	
}

void AEnemyBaseCharacter::InitializeEnemyData(FEnemyDataRow& InData)
{
	EnemyData.EnemyType = InData.EnemyType;
	EnemyData.MaxHealth = InData.MaxHP;
	EnemyData.CurrentHealth = InData.MaxHP;
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


void AEnemyBaseCharacter::ChangeEnemyState(EEnemyState NewEnemyState)
{
	EnemyState = NewEnemyState;

	OnEnemyStateChanged.ExecuteIfBound(EnemyState);
}

void AEnemyBaseCharacter::SetEnemyNoCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
