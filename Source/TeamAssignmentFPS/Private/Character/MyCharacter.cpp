// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "Camera/CameraManager.h"
#include "InputAction.h"

// components
#include "MaterialHLSLTree.h"
#include "Components/TimelineComponent.h"
#include "LockonTarget/LockonComponent.h"
#include "Weapon/EquipmentManagerCompnent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterStat/HealthComponent.h"

#include "Item/InventoryManagerComponent.h"

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
	EquipmentInteractionComp=CreateDefaultSubobject<UEquipmentManagerCompnent>(TEXT("EquipmentManager Component"));
	InventoryComp=CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("Inventory Component"));
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

	//temp rotaion without state case
	RotateTowardTarget(DeltaTime);

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
	FVector Forward = FVector::ZeroVector;
	FVector Right = FVector::ZeroVector;
	FVector Up = FVector::ZeroVector;
	FVector GravityDirection = FVector(0,0,-1);

	if (CameraManagerComp && CameraManagerComp->GetVectorsByCameraAndGravityDirection(GravityDirection, Forward, Right, Up))
	{
		if (!FMath::IsNearlyZero(MovementInputValue.X))// forward
		{
			AddMovementInput(Forward, MovementInputValue.X, false);
			UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Forward : %f"),MovementInputValue.X)
		}
		if (!FMath::IsNearlyZero(MovementInputValue.Y))// right
		{
			AddMovementInput(Right, MovementInputValue.Y, false);
			UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Right : %f"),MovementInputValue.Y)
		}
	}

	UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Function is running"));
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

	float RoationInterpSpeed=6.f;// temp

	FRotator NewRotation=FMath::RInterpTo(GetActorRotation(),TargetRotatioin,Deltatime,RoationInterpSpeed);
	SetActorRotation(NewRotation);
	
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

