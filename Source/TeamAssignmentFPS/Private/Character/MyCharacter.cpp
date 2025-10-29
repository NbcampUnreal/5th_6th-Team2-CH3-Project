// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "Camera/CameraManager.h"
#include "InputAction.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"
#include "LockonTarget/LockonComponent.h"
#include "Debug/UELOGCategories.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/HealthComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LockonComp=CreateDefaultSubobject<ULockonComponent>(TEXT("LockonComponent"));
	CameraManagerComp=CreateDefaultSubobject<UCameraManagerComp>(TEXT("CameraManagerComponent"));
	HealthComponent=CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyCharacter::SetupForDodgeAction()
{
	if (!DodgeTimeLine)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::SetupFordodge-> Invalid DodgeTimeLine"));
		return;
	}
	if (!DodgeCurve)// it must be a 0 to 1 alpha curve!!!!!
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::SetupFordodge-> Invalid DodgeCurve"));
		return;
	}

	FOnTimelineFloat ProgressFunction;// why ufucntion is not working?
	ProgressFunction.BindUFunction(this, FName("HandleDodgeAction"));
	DodgeTimeLine->AddInterpFloat(DodgeCurve, ProgressFunction);

	FOnTimelineEvent FinishFunction;
	FinishFunction.BindUFunction(this, FName("OnDodgeFinished"));
	DodgeTimeLine->SetTimelineFinishedFunc(FinishFunction);
	
	
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

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyCharacter::SetMovementState(ECharacterMovementState NewMovementState)
{
	if (CurrentMovementState == NewMovementState)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::SetMovementState-> Same MovementState"))
		return;
	}
	
	/*CurrentMovementState = NewMovementState;
	//need to signal the state update.*/
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
	FVector Forward, Right, Up;
	FVector GravityDirection = FVector(0,0,-1);

	if (CameraManagerComp && CameraManagerComp->GetVectorsByCameraAndGravityDirection(GravityDirection, Forward, Right, Up))
	{
		AddMovementInput(Forward, MovementInputValue.X);
		AddMovementInput(Right, MovementInputValue.Y);
	}

	if (!FMath::IsNearlyZero(MovementInputValue.X))// forward
	{
		AddMovementInput(Forward, MovementInputValue.X,false);
		UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Forward : %f"),MovementInputValue.X)
		// movement direction, scale, forced or not
	}
	if (!FMath::IsNearlyZero(MovementInputValue.Y))// right
	{
		AddMovementInput(Right, MovementInputValue.Y);
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::MoveForwardAndRight-> Right : %f"),MovementInputValue.Y)
	}

	UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Function is running"));
}

void AMyCharacter::RotateTowardTarget(float Deltatime)
{
	
}

void AMyCharacter::StartSprinting()
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::StartSprinting-> Invalid Controller"))
		return;
	}
	CurrentMaxSpeed=MovementSpeed*SprintSpeedMultiplier;
	
	GetCharacterMovement()->MaxWalkSpeed=CurrentMaxSpeed;
	LockonComp->SetCameraBlendAlpha(0.f);// camera to character base

	
}

void AMyCharacter::StopSprinting()
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::StartSprinting-> Invalid Controller"))
		return;
	}
	CurrentMaxSpeed=MovementSpeed;
	
	GetCharacterMovement()->MaxWalkSpeed=CurrentMaxSpeed;
	LockonComp->SetCameraBlendAlpha(0.2f);//temp--> find a way to reset to its value

}

void AMyCharacter::Dodge(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->Velocity.Size()==KINDA_SMALL_NUMBER)//when speed is nearly zero
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
	FVector Forward, Right, Up;
	FVector GravityDirection = FVector(0,0,-1);

	if (CameraManagerComp && CameraManagerComp->GetVectorsByCameraAndGravityDirection(GravityDirection, Forward, Right, Up))
	{
		//launch character to the current input direction
		//float DodgeDistance=600;// use timeline for the dodge update so that the 
		
	}
}

void AMyCharacter::BackDash()
{
	//launch character to the back
}

void AMyCharacter::HandleDodgeAction(float DeltaTime)
{
}

void AMyCharacter::OnDodgeFinished()
{
	bIsDodging=false;
	GetCharacterMovement()->Velocity=FVector::ZeroVector;
}

void AMyCharacter::TriggerBattleAction()
{
}

void AMyCharacter::SwitchAction(const FInputActionValue& Value)
{
}

void AMyCharacter::UseWeapon()
{
}

void AMyCharacter::UseItem()
{
}

void AMyCharacter::SelectItem()
{
}

void AMyCharacter::SwitchToNewWeapon()
{
}

void AMyCharacter::SwtichToWeapon()
{
}

void AMyCharacter::LockonTarget()
{
}

