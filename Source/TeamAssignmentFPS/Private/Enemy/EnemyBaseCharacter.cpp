
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

void AEnemyBaseCharacter::EnemyAttack()
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack"));

	EnemyState = EEnemyState::EES_Attack;

	ChangeEnemyState(EnemyState);

	FTimerHandle TestTimerHandle;

	GetWorldTimerManager().SetTimer(TestTimerHandle, this, &AEnemyBaseCharacter::EnemyAttackEnd, 0.2f);

}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemyBaseCharacter::EnemyAttackEnd()
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack End"));

	ChangeEnemyState(EEnemyState::EES_Chase);
}

float AEnemyBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	IInterfaceHP* InterfaceHP = Cast<IInterfaceHP>(HealthComponent);

	FDamageInfo DamageInfo;
	DamageInfo.DamageAmount = Damage;

	InterfaceHP->GetDamage_Implementation(DamageInfo);
	
	return 0.0f;
}

void AEnemyBaseCharacter::ChangeEnemyState(EEnemyState NewEnemyState)
{
	EnemyState = EEnemyState::EES_Chase;

	OnEnemyStateChanged.ExecuteIfBound(EnemyState);
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



