#include "Enemy/EnemyBaseCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "CharacterStat/HealthComponent.h"
#include "Enemy/EnemyState/EnemyDataRow.h"
#include "GameState/GameStateManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pooling/PoolingSubsystem.h"

AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true);
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	EnemyState = EEnemyState::EES_None;

}
void AEnemyBaseCharacter::InitializeEnemyData(FEnemyDataRow* InData)
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Data Initialized"));
	
	EnemyData.EnemyType = InData->EnemyType;
	HealthComponent->SetMaxHealth(InData->MaxHP);
	HealthComponent->SetCurrentHealth(InData->MaxHP);
	EnemyData.MoveSpeed = InData->MoveSpeed;
	EnemyData.HeightMinRatio = InData->HeightMinRatio;
	EnemyData.HeightMaxRatio = InData->HeightMaxRatio;
	EnemyData.Damage = InData->Damage;
	EnemyData.Range = InData->Range;
	EnemyData.Delay = InData->Delay;
	EnemyData.Score = InData->Score;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bOrientRotationToMovement = true;

	Movement->MaxWalkSpeed = EnemyData.MoveSpeed;
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnDeath.AddUObject(this, &AEnemyBaseCharacter::EnemyDead);
	HealthComponent->OnDamage.BindUObject(this, &AEnemyBaseCharacter::EnemyTakeDamage);
	
	AGameStateManager* GameStateManager = Cast<AGameStateManager>(GetWorld()->GetGameState());
	if (GameStateManager)
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		UE_LOG(Enemy_Log, Error, TEXT("GameStateManager Found"));
		GameStateManager->PhaseOver.AddDynamic(this, &AEnemyBaseCharacter::EnemyDeadByPhaseEnd);
=======
		GameStateManager->PhaseOver.AddDynamic(this, &AEnemyBaseCharacter::EnemyDead);
>>>>>>> f351064 (chore)
=======
		GameStateManager->PhaseOver.AddDynamic(this, &AEnemyBaseCharacter::EnemyDestroy);
>>>>>>> b8ccb7b (feat enemy anim)
=======
		UE_LOG(Enemy_Log, Error, TEXT("GameStateManager Found"));
		GameStateManager->PhaseOver.AddDynamic(this, &AEnemyBaseCharacter::EnemyDeadByPhaseEnd);
>>>>>>> 83cc9c5 (delete)
	}
}

void AEnemyBaseCharacter::EnemyAttack()
{
	//UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack"));

	if (EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Attack)
	{
		return;
	}

	if (bCanAttack == false)
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

void AEnemyBaseCharacter::EnemyTakeDamage(FDamageInfo DamageInfo)
{
	//������ ���� �� ȣ���� �Լ�

	UE_LOG(Enemy_Log, Error, TEXT("Enemy Damaged"));

	if (GetEnemyState() == EEnemyState::EES_Spawn)
	{
		return;
	}
	
	//Knockback();
	
	ChangeEnemyState(EEnemyState::EES_Damaged);
	
}

// void AEnemyBaseCharacter::Knockback()
// {
// 	if (EnemyState == EEnemyState::EES_Dead)
// 	{
// 		return;
// 	}
// 	FVector D = GetActorForwardVector();
// 	SetActorLocation(GetActorLocation()+ D * 100.f);
// 	
// }

void AEnemyBaseCharacter::EnemyDead(FDamageInfo DamageInfo)
{
	
	//DisableEnemyCollision();
	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	ChangeEnemyState(EEnemyState::EES_Dead);
	
<<<<<<< HEAD
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 0f253c7 (Reapply "murge into seo")
		bool WasDestoryed;
		if (!PoolingSubsystem->ReturnToPoolOrDestroy(this,WasDestoryed))
		{
			//Error, Failed to Return Enemy actor or destory it.
			return;
		}

		// if if worked

		FString EnemyName=this->GetName();
		FString LogText=WasDestoryed? TEXT("Destoryed"):TEXT("Returned to pool");
		UE_LOG(Enemy_Log, Log,TEXT("AEnemyBaseCharacter::EnemyDead-> Enemy [%s] is dead and %s"),*EnemyName,*LogText);
<<<<<<< HEAD
=======
=======
>>>>>>> cccbfc2 (Revert "murge into seo")
=======
>>>>>>> 0f253c7 (Reapply "murge into seo")
		AIController->StopBehaviorTree();
>>>>>>> b8ccb7b (feat enemy anim)
	}
=======
	//Destroy();
>>>>>>> parent of b8ccb7b (feat enemy anim)

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		PoolingSubsystem->ReturnToPool(this);
	}
	
}

void AEnemyBaseCharacter::EnemyDeadByPhaseEnd()
{
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Dead by phase over"));

	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	ChangeEnemyState(EEnemyState::EES_Dead);

	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
	{
		bool WasDestoryed;
		if (!PoolingSubsystem->ReturnToPoolOrDestroy(this,WasDestoryed))
		{
			//Error, Failed to Return Enemy actor or destory it.
			return;
		}

		FString EnemyName=this->GetName();
		FString LogText=WasDestoryed? TEXT("Destoryed"):TEXT("Returned to pool");
		UE_LOG(Enemy_Log, Log,TEXT("AEnemyBaseCharacter::EnemyDead-> Enemy [%s] is dead and %s"),*EnemyName,*LogText);
	}
}


void AEnemyBaseCharacter::ChangeEnemyState(EEnemyState NewEnemyState)
{
	EnemyState = NewEnemyState;

	OnEnemyStateChanged.Broadcast(EnemyState);
}

void AEnemyBaseCharacter::EndEnemySpawn()
{
	ChangeEnemyState(EEnemyState::EES_Chase);
	UE_LOG(Enemy_Log, Error, TEXT("Enemy Spawn End"));
}

void AEnemyBaseCharacter::EndChase()
{
	ChangeEnemyState(EEnemyState::EES_Idle);
	if (bCanAttack == false)
	{
		return;
	}
	EnemyAttack();
}

void AEnemyBaseCharacter::DisableEnemyCollision()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemyBaseCharacter::OnSpawnFromPool_Implementation()
{
	IPoolingInterface::OnSpawnFromPool_Implementation();

	ChangeEnemyState(EEnemyState::EES_Spawn);
	LookAtPlayer();
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->StartBehaviorTree();
	}
	EndEnemySpawn();
}

void AEnemyBaseCharacter::OnReturnToPool_Implementation()
{
	IPoolingInterface::OnReturnToPool_Implementation();

	ChangeEnemyState(EEnemyState::EES_None);
	
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->StopBehaviorTree();
	}
	
}

void AEnemyBaseCharacter::LookAtPlayer()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!Player)
	{
		return;
	}

	FRotator TargetRotation	= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
	TargetRotation.Roll = 0.f;
	TargetRotation.Pitch = 0.f;

	SetActorRotation(TargetRotation);
}
