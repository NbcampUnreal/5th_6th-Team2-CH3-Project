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

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	bUseControllerRotationYaw = false;	
	Movement->RotationRate = FRotator(0.f, 540.f, 0.f);
	Movement->bOrientRotationToMovement = true;
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
	Movement->MaxWalkSpeed = EnemyData.MoveSpeed;

	float RandomValue = FMath::RandRange(EnemyData.HeightMinRatio, EnemyData.HeightMaxRatio);
	SetActorScale3D(GetActorScale3D()* RandomValue);
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
=======
		GameStateManager->PhaseOver.AddDynamic(this, &AEnemyBaseCharacter::EnemyDestroy);
>>>>>>> 652a79a (Revert "delete")
	}
}

void AEnemyBaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UE_LOG(LogTemp, Error, TEXT("range %f"), EnemyData.Range);
}

void AEnemyBaseCharacter::EndEnemySpawn()
{
	ChangeEnemyState(EEnemyState::EES_Chase);

	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->StartBehaviorTree();
		
	}

	
}

void AEnemyBaseCharacter::EndChase()
{
	//ChangeEnemyState(EEnemyState::EES_Idle);
	// if (bCanAttack == false)
	// {
	// 	return;
	// }
	EnemyAttack();
}

void AEnemyBaseCharacter::EnemyAttack()
{
	//UE_LOG(Enemy_Log, Error, TEXT("Enemy Attack"));

	if (EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Attack)
	{
		return;
	}

	//LookAtPlayer();
	
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->SetCanAttackRotate(true);
	}

	ChangeEnemyState(EEnemyState::EES_Attack);
}

void AEnemyBaseCharacter::EndEnemyAttack()
{
	ChangeEnemyState(EEnemyState::EES_Idle);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBaseCharacter::ReturnToChase, EnemyData.Delay, false);
}

void AEnemyBaseCharacter::ReturnToChase()
{
	ChangeEnemyState(EEnemyState::EES_Chase);
}

void AEnemyBaseCharacter::EnemyTakeDamage(FDamageInfo DamageInfo)
{
	//������ ���� �� ȣ���� �Լ�
	
	HitDirection = DamageInfo.DamageDirection;
	
	if (GetEnemyState() == EEnemyState::EES_Spawn || GetEnemyState() == EEnemyState::EES_Damaged || GetEnemyState() == EEnemyState::EES_Dead)
	{
		return;
	}

	
	PlayHitMontage(HitReactMontage);
	
	ChangeEnemyState(EEnemyState::EES_Damaged);
	
}

double AEnemyBaseCharacter::GetKnockbackDireation(FVector Direction)
{
	const FVector ForwardVector = GetActorForwardVector();
	
	const FVector ToHit  = Direction.GetSafeNormal();
	
	const double CosTheta = FVector::DotProduct(ForwardVector, ToHit);
	double Theta = FMath::Acos(CosTheta);

	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;	
	}

	return Theta;
}

void AEnemyBaseCharacter::EndHitReact()
{
	ChangeEnemyState(EEnemyState::EES_Chase);
}


void AEnemyBaseCharacter::EnemyDead(FDamageInfo DamageInfo)
{
	//DisableEnemyCollision();
	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	SetActorEnableCollision(false);
	//StopMontage(HitReactMontage);
	PlayMontage(DeadMontage);
	ChangeEnemyState(EEnemyState::EES_Dead);
    
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 0f253c7 (Reapply "murge into seo")
		bool WasDestoryed;
		if (!PoolingSubsystem->ReturnToPoolOrDestroy(this,WasDestoryed))
=======
    
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
>>>>>>> d74bc04 (fix)
		{
			bool WasDestoryed;
			if (!PoolingSubsystem->ReturnToPoolOrDestroy(this,WasDestoryed))
			{
				//Error, Failed to Return Enemy actor or destroy it.
				return;
			}
			// if if worked

			FString EnemyName=this->GetName();
			FString LogText=WasDestoryed? TEXT("Destroyed"):TEXT("Returned to pool");
			UE_LOG(Enemy_Log, Log,TEXT("AEnemyBaseCharacter::EnemyDead-> Enemy [%s] is dead and %s"),*EnemyName,*LogText);
			AIController->StopBehaviorTree();
		}
<<<<<<< HEAD

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
=======
>>>>>>> d74bc04 (fix)
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBaseCharacter::EnemyDestroy, 10.f, false);
    
}
void AEnemyBaseCharacter::EndEnemyDead()
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

void AEnemyBaseCharacter::EnemyDestroy()
{
	ChangeEnemyState(EEnemyState::EES_None);
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


// void AEnemyBaseCharacter::DisableEnemyCollision()
// {
// 	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// }

void AEnemyBaseCharacter::OnSpawnFromPool_Implementation()
{
	IPoolingInterface::OnSpawnFromPool_Implementation();

	ChangeEnemyState(EEnemyState::EES_Spawn);
	SetActorRotation(LookAtPlayer());
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		
		FTimerHandle DelayHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DelayHandle,
			[this, AIController]()
			{
				if (AIController)
				{
					AIController->DefaultSettingBlackBoard();
					
				}
			},
			0.2f,  
			false  
		);

		
	}

}

void AEnemyBaseCharacter::OnReturnToPool_Implementation()
{
	IPoolingInterface::OnReturnToPool_Implementation();

	ChangeEnemyState(EEnemyState::EES_None);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
		AIController->StopBehaviorTree();
	}
	
}


void AEnemyBaseCharacter::PlayHitMontage(UAnimMontage* Montage)
{
	double Theta = GetKnockbackDireation(HitDirection);

	UE_LOG(Enemy_Log, Warning, TEXT("%f"), Theta);
	
	FName SectionName = FName("Front");

	if (Theta >= -45.f && Theta < 45.f)
	{
		SectionName = FName("Back");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		SectionName = FName("Right");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		SectionName = FName("Left");
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void AEnemyBaseCharacter::PlayMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
	}
}

void AEnemyBaseCharacter::StopMontage(UAnimMontage* Montage)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Stop(-1.f, Montage);
	}
}

void AEnemyBaseCharacter::PlaySound(USoundBase* Sound)
{
	UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
}

FRotator AEnemyBaseCharacter::LookAtPlayer()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!Player)
	{
		return FRotator::ZeroRotator;
	}

	FRotator TargetRotation	= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());
	TargetRotation.Roll = 0.f;
	TargetRotation.Pitch = 0.f;

	return TargetRotation;
}



