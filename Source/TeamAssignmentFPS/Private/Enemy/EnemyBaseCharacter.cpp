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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
		UE_LOG(Enemy_Log, Error, TEXT("GameStateManager Found"));
		GameStateManager->PhaseOver.AddDynamic(this, &AEnemyBaseCharacter::EnemyDeadByPhaseEnd);
=======
=======
>>>>>>> f351064 (chore)
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
<<<<<<< HEAD

	// Set TargetActor
	AActor* NewTargetActor= UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!NewTargetActor)
	{
		UE_LOG(Enemy_Log, Error, TEXT("AEnemyBaseCharacter::BeginPlay-> invalid target actor"));
	}
	else
	{
		SetTargetActor(NewTargetActor);
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
=======
	
	AActor* Player=GetWorld()->GetFirstPlayerController()->GetPawn();

	SetTargetActor(Player);
>>>>>>> f79331c (weapon updated)
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

<<<<<<< HEAD
<<<<<<< HEAD
double AEnemyBaseCharacter::GetKnockbackDireation(FVector Direction)
<<<<<<< HEAD
{
<<<<<<< HEAD
	const FVector ForwardVector = GetActorForwardVector();
=======
	
=======
=======
double AEnemyBaseCharacter::GetKnockbackDireation(FVector Direction)
>>>>>>> 652a79a (Revert "delete")
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


<<<<<<< HEAD
<<<<<<< HEAD

void AEnemyBaseCharacter::EnemyDead(FDamageInfo DamageInfo)
{
>>>>>>> b8ccb7b (feat enemy anim)
=======
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
	
>>>>>>> 83cc9c5 (delete)
=======

void AEnemyBaseCharacter::EnemyDead(FDamageInfo DamageInfo)
{
>>>>>>> 652a79a (Revert "delete")
	//DisableEnemyCollision();
	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	SetActorEnableCollision(false);
	//StopMontage(HitReactMontage);
	PlayMontage(DeadMontage);
	ChangeEnemyState(EEnemyState::EES_Dead);
>>>>>>> f351064 (chore)
	
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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


=======
>>>>>>> d74bc04 (fix)
void AEnemyBaseCharacter::EnemyDead(FDamageInfo DamageInfo)
{
	//DisableEnemyCollision();
	OnEnemyDead.ExecuteIfBound(GetEnemyData().Score);

	SetActorEnableCollision(false);
	//StopMontage(HitReactMontage);
	PlayMontage(DeadMontage);
	ChangeEnemyState(EEnemyState::EES_Dead);
    
<<<<<<< HEAD
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
=======
=======
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
=======
>>>>>>> d74bc04 (fix)
	if (AEnemyAIController* AIController = Cast<AEnemyAIController>(GetController()))
	{
<<<<<<< HEAD
    
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
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
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBaseCharacter::EnemyDestroy, 10.f, false);
    
}
void AEnemyBaseCharacter::EndEnemyDead()
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

void AEnemyBaseCharacter::SetTargetActor(AActor* NewTargetActor)
{
	if (!NewTargetActor)
	{
		UE_LOG(Enemy_Log, Error, TEXT(" AEnemyBaseCharacter::SetTargetActor-> Invalid Target"));
		return;
	}

	if (NewTargetActor==TargetActor)
	{
		UE_LOG(Enemy_Log, Warning, TEXT(" AEnemyBaseCharacter::SetTargetActor-> Already Setted to same target"));
		return;
	}

	TargetActor=NewTargetActor;
	UE_LOG(Enemy_Log, Log, TEXT("AEnemyBaseCharacter::SetTargetActor-> Target Set to %s"), *TargetActor->GetName())
}

void AEnemyBaseCharacter::EnemyDestroy()
{
	ChangeEnemyState(EEnemyState::EES_None);
	if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
>>>>>>> b8ccb7b (feat enemy anim)
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 0f253c7 (Reapply "murge into seo")
		bool WasDestoryed;
		if (!PoolingSubsystem->ReturnToPoolOrDestroy(this,WasDestoryed))
=======
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
    
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
>>>>>>> d74bc04 (fix)
=======
	
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
>>>>>>> abfb1f3 (update)
=======
	
		if (UPoolingSubsystem* PoolingSubsystem = GetWorld()->GetSubsystem<UPoolingSubsystem>())
>>>>>>> abfb1f3 (update)
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
<<<<<<< HEAD
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
=======
>>>>>>> abfb1f3 (update)
=======
>>>>>>> abfb1f3 (update)
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBaseCharacter::EnemyDestroy, 10.f, false);
    
}
void AEnemyBaseCharacter::EndEnemyDead()
{
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
}

<<<<<<< HEAD
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

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b8ccb7b (feat enemy anim)
=======

>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
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
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b8ccb7b (feat enemy anim)
=======
>>>>>>> 652a79a (Revert "delete")
		
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

		
<<<<<<< HEAD
<<<<<<< HEAD
	}

=======
		AIController->StartBehaviorTree();
	}
	EndEnemySpawn();
>>>>>>> f351064 (chore)
=======
	}

>>>>>>> b8ccb7b (feat enemy anim)
=======
		AIController->StartBehaviorTree();
	}
	EndEnemySpawn();
>>>>>>> 83cc9c5 (delete)
=======
	}

>>>>>>> 652a79a (Revert "delete")
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

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 652a79a (Revert "delete")

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
<<<<<<< HEAD
=======
void AEnemyBaseCharacter::LookAtPlayer()
>>>>>>> 83cc9c5 (delete)
=======
>>>>>>> 652a79a (Revert "delete")
{
<<<<<<< HEAD
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
>>>>>>> b8ccb7b (feat enemy anim)

void AEnemyBaseCharacter::PlayHitMontage(UAnimMontage* Montage)
{
	double Theta = GetKnockbackDireation(HitDirection);

	UE_LOG(Enemy_Log, Warning, TEXT("%f"), Theta);
	
	FName SectionName = FName("Front");

	if (Theta >= -45.f && Theta < 45.f)
=======
	//APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (!TargetActor)
>>>>>>> f79331c (weapon updated)
	{
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
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
	if (!TargetActor)
	{
=======
>>>>>>> b8ccb7b (feat enemy anim)
		return FRotator::ZeroRotator;
=======
		return;
>>>>>>> 83cc9c5 (delete)
=======
		return FRotator::ZeroRotator;
>>>>>>> 652a79a (Revert "delete")
	}

	FRotator TargetRotation	= UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	TargetRotation.Roll = 0.f;
	TargetRotation.Pitch = 0.f;

	return TargetRotation;
}



