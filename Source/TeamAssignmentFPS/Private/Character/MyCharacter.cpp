// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "Camera/CameraManager.h"
#include "InputAction.h"

// components
#include "Components/TimelineComponent.h"
#include "LockonTarget/LockonComponent.h"
#include "Weapon/EquipmentManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/HealthComponent.h"
#include "Item/InteractionComponent.h"
#include "InputHelper/InputActionHandler.h"


#include "Curves/CurveFloat.h"
#include "Debug/UELOGCategories.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LockonComp=CreateDefaultSubobject<ULockonComponent>(TEXT("Lockon Component"));
	CameraManagerComp=CreateDefaultSubobject<UCameraManagerComponent>(TEXT("CameraManager Component"));
	HealthComponent=CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	EquipmentInteractionComp=CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("EquipmentManager Component"));
	InteractionComp=CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	
	DodgeTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DodgeTimeline"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//sprint, dodge input action setting
	SetupForDodgeAction();
	SetupForInputTypeHelper();

	if (InteractionComp)//activate interaction comp
	{
		InteractionComp->SetActivationForInteractionComponent(true);
	}
	
}

void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	//Delete unnecessary
	//1. Helper
	if (DodgeInputDetectionHelper)
	{
		DodgeInputDetectionHelper->RemoveFromRoot();
		DodgeInputDetectionHelper=nullptr;
	}
	
}

void AMyCharacter::SetupForDodgeAction()
{
	if (!DodgeTimeline)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::SetupForDodge-> Invalid DodgeTimeLine"));
		return;
	}
	if (!DodgeCurve)// it must be a 0 to 1 alpha curve!!!!!
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::SetupForDodge-> Invalid DodgeCurve"));
		return;
	}
	
	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("HandleDodgeAction"));
	DodgeTimeline->AddInterpFloat(DodgeCurve, ProgressFunction);

	FOnTimelineEvent FinishFunction;
	FinishFunction.BindUFunction(this, FName("OnDodgeFinished"));
	DodgeTimeline->SetTimelineFinishedFunc(FinishFunction);
}

void AMyCharacter::SetupForInputTypeHelper()
{
	//create it first
	DodgeInputDetectionHelper=NewObject<UInputActionHandler>(this, UInputActionHandler::StaticClass());
	
	if (!DodgeInputDetectionHelper)
	{
		UE_LOG(Movement_Log, Error,
			TEXT("void AMyCharacter::BindActionsForInputTypeHelper-> Invalid Helper, Binding Failed"));
		return;
	}
	DodgeInputDetectionHelper->AddToRoot();//for safety--> need to remove when leave

	DodgeInputDetectionHelper->SetShouldTriggerWhenCanceled(false);// do nothing when the trigger is canceled
	
	DodgeInputDetectionHelper->OnTapped.BindUObject(this, &AMyCharacter::Dodge);
	DodgeInputDetectionHelper->OnHoldStart.BindUObject(this, &AMyCharacter::StartSprinting);
	DodgeInputDetectionHelper->OnReleased.BindUObject(this, &AMyCharacter::StopSprinting);
	UE_LOG(Movement_Log, Log,
			TEXT("void AMyCharacter::BindActionsForInputTypeHelper-> Invalid Helper, Binding Completed"));
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// condition to set the alpha blend to 0 so that camera base is located on the center
	if (CurrentMovementState==ECharacterMovementState::Sprinting)
	{
		//LockonComp->set
	}

	//temp rotaion without state case
	RotateTowardTarget(DeltaTime);
	//temp with simple trigger

	//evaluate what movement state character has right now
	DecideMovementState();
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::MoveForwardAndRight(const FInputActionValue& Value)
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::MoveForwardAndRight-> Invalid Controller"))
		return;
	}
	
	MovementInputValue=Value.Get<FVector2D>();

	//Get Forward and right vector from camera manager
	FVector Forward = FVector::ZeroVector;
	FVector Right = FVector::ZeroVector;
	FVector Up = FVector::ZeroVector;
	FVector GravityDirection = FVector(0,0,-1);

	if (CameraManagerComp && CameraManagerComp->GetVectorsByCameraAndGravityDirection(GravityDirection, Forward, Right, Up))
	{
		if (!FMath::IsNearlyZero(MovementInputValue.X))// forward
		{
			AddMovementInput(Forward, MovementInputValue.X, false);
			//UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Forward : %f"),MovementInputValue.X);
		}
		if (!FMath::IsNearlyZero(MovementInputValue.Y))// right
		{
			AddMovementInput(Right, MovementInputValue.Y, false);
			//UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Right : %f"),MovementInputValue.Y);
		}
	}
}

void AMyCharacter::RotateTowardTarget(float Deltatime)
{
	FVector CursorLocation;
	if (!LockonComp->GetDeprojectedCursorLocation(CursorLocation))
	{
		// not activated, so, no cursor location is valid location
		return;
	}
	// if the cursorlocation is valid location

	FVector LookAtDirection=CursorLocation-GetActorLocation();

	float TooCloseDistance=100.f;// temp. this is for when the cursor is too close to the player character
	if (LookAtDirection.Size()<TooCloseDistance)
	{
		// too cloase, do nothing
		return;
	}
	
	FRotator TargetRotatioin=LookAtDirection.Rotation();
	//lock the pitch, roll rotoation(roll for just incase)
	TargetRotatioin.Pitch=GetActorRotation().Pitch;
	TargetRotatioin.Roll=GetActorRotation().Roll;

	FRotator NewRotation=FMath::RInterpTo(GetActorRotation(),TargetRotatioin,Deltatime,RotationInterpSpeed);
	SetActorRotation(NewRotation);
}

void AMyCharacter::TriggerQuickMovement_Pressed(const FInputActionValue& Value)
{
	float FloatValue=Value.Get<float>();
	if (!DodgeInputDetectionHelper) return;
	DodgeInputDetectionHelper->OnTriggerPressed(FloatValue);
}

void AMyCharacter::TriggerQuickMovement_Released(const FInputActionValue& Value)
{
	if (!DodgeInputDetectionHelper) return;
	DodgeInputDetectionHelper->OnTriggerCompleted();
}

void AMyCharacter::TriggerQuickMovement_Canceled(const FInputActionValue& Value)
{
	if (!DodgeInputDetectionHelper) return;
	DodgeInputDetectionHelper->OnTriggerCanceled();
}

void AMyCharacter::StartSprinting()
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::StartSprinting-> Invalid Controller"))
		return;
	}
	CurrentMaxSpeed=MovementSpeed*SprintSpeedMultiplier;
	UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::StartSprinting-> Start Sprint"));
	
	GetCharacterMovement()->MaxWalkSpeed=CurrentMaxSpeed;
	LockonComp->SetCameraBlendAlpha(0.f);// camera to character base

	SetMovementState(ECharacterMovementState::Sprinting);
}

void AMyCharacter::StopSprinting()
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::StartSprinting-> Invalid Controller"))
		return;
	}
	CurrentMaxSpeed=MovementSpeed;
	UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::StopSprinting-> Stop Sprinting"));
	
	GetCharacterMovement()->MaxWalkSpeed=CurrentMaxSpeed;
	LockonComp->SetCameraBlendAlpha(0.2f);//temp--> find a way to reset to its value

	SetMovementState(ECharacterMovementState::Idle);

}

void AMyCharacter::Dodge()
{
	if (bIsDodging)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::Dodge-> Dodge already in progress"));
		return;
	}
	float MinSpeedForDirectionalDodge=10.f;//temp
	UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::Dodge-> Start Dodge"));
	
	if (GetCharacterMovement()->Velocity.Size()<=MinSpeedForDirectionalDodge)//when speed is nearly zero
	{
		BackDash();
	}
	else
	{
		DirectionalDodge();
	}
}

void AMyCharacter::DirectionalDodge()
{
	if (!DodgeCurve || !DodgeTimeline)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::DirectionalDodge -> Missing curve or timeline"));
		return;
	}
	
	FVector Forward, Right, Up;
	FVector GravityDirection = FVector(0,0,-1);
	
	if (!CameraManagerComp || !CameraManagerComp->GetVectorsByCameraAndGravityDirection(GravityDirection, Forward, Right, Up))
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::DirectionalDodge -> Cannot Calculate vectors based on camera"));
		return;
	}
	const FVector2D Input = MovementInputValue;
	DodgeDirection = (Forward * Input.X + Right * Input.Y).GetSafeNormal();
	
	bIsDodging = true;
	DodgeTimeline->SetPlayRate(FMath::Max(0.01f/*Min value for safety*/, DodgeSpeedPlayRate));
	DodgeTimeline->PlayFromStart();

	SetMovementState(ECharacterMovementState::Dodging);//set movement state here
}

void AMyCharacter::BackDash()
{
	if (!DodgeCurve || !DodgeTimeline)
	{
		UE_LOG(Movement_Log, Error, TEXT("BackDash -> Missing curve or timeline"));
		return;
	}

	DodgeDirection = -GetActorForwardVector() * BackDashDistanceRatio;

	bIsDodging = true;
	DodgeTimeline->SetPlayRate(FMath::Max(0.01f, DodgeSpeedPlayRate*BackDashTimeRatio));
	DodgeTimeline->PlayFromStart();

	SetMovementState(ECharacterMovementState::Dodging);
}

void AMyCharacter::HandleDodgeAction(float DeltaTime)
{
	if (!bIsDodging)
		return;
	
	const FVector DodgeOffset = DodgeDirection * DodgeDistance * DeltaTime;

	FVector NewLocation = GetActorLocation() + DodgeOffset * GetWorld()->GetDeltaSeconds();
	SetActorLocation(NewLocation, true);
}

void AMyCharacter::OnDodgeFinished()
{
	bIsDodging = false;
	GetCharacterMovement()->Velocity = FVector::ZeroVector;
	SetMovementState(ECharacterMovementState::Idle);
}

void AMyCharacter::OnDagamed(FDamageInfo Damage)
{
	KockBackCharacter(Damage);

	// additional effects
	// camera shake
	//
	
}

void AMyCharacter::OnDeath(FDamageInfo Damage)
{
	// do what death does

	KockBackCharacter(Damage);
	// add Death widget
	// disable controll
	//etc
}

void AMyCharacter::KockBackCharacter(FDamageInfo Damage)
{
	//knock back
	if (!Damage.DamageDirection.IsNearlyZero())
	{
		//error, direction invalid
		return;
	}

	FVector KnockBackDirection=Damage.DamageDirection.GetSafeNormal();
	
	float KnockBackAmountMultiplier=50.f;//temp
	float MaxKnockBackForce=1000.f;//temp
	
	float KnockBackForce=FMath::Clamp(Damage.DamageAmount*KnockBackAmountMultiplier,0.f,MaxKnockBackForce);
	

	LaunchCharacter(KnockBackDirection*KnockBackForce, true, true);
}

void AMyCharacter::HealthComponentSetup()
{
	if (!HealthComponent)
	{
		//error, invalid hpcomp
		return;
	}
	
	HealthComponent->OnDamage.BindUObject(this, &AMyCharacter::OnDagamed);
	HealthComponent->OnDeath.AddUObject(this, &AMyCharacter::OnDeath);
}

void AMyCharacter::HandleOnMovementStateChanged()
{
	
}

void AMyCharacter::DecideMovementState()
{
	if (bIsDodging)
	{
		SetMovementState(ECharacterMovementState::Dodging);
		return;
	}
	const UCharacterMovementComponent* MovementComponent = GetCharacterMovement();

	if (!MovementComponent) return;// invalid movement comp

	//== Mid-Air Conditions
	if (MovementComponent->IsFalling())
	{
		SetMovementState(ECharacterMovementState::Falling);
		return;
	}

	//=== Based on Speed

	float Speed = MovementComponent->Velocity.Size();
	bool bIsCrouching = MovementComponent->IsCrouching();
	float MinSpeedForMove=5.f;
	if (Speed<MinSpeedForMove)
	{
		SetMovementState(ECharacterMovementState::Idle);
	}
	else//not idle, moving
	{
		if (Speed>MovementSpeed && bIsSprinting)
		{
			SetMovementState(ECharacterMovementState::Sprinting);
		}
		else
		{
			SetMovementState(ECharacterMovementState::Moving);
		}
	}
}

void AMyCharacter::SetMovementState(ECharacterMovementState NewState)
{
	
}


