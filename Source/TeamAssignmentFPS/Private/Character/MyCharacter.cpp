// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "Camera/CameraManager.h"
#include "InputAction.h"
#include "LockonTarget/LockonComponent.h"
#include "Debug/UELOGCategories.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LockonComp=CreateDefaultSubobject<ULockonComponent>(TEXT("LockonComponent"));
	//CameraManagerComp=CreateDefaultSubobject<UCameraManagerComp>(TEXT("CameraManagerComponent"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	
	const FVector2D MoveInput=Value.Get<FVector2D>();

	if (!FMath::IsNearlyZero(MoveInput.X))// forward
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X,false);
		UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Forward : %f"),MoveInput.X)
		// movement direction, scale, forced or not
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))// right
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::MoveForwardAndRight-> Right : %f"),MoveInput.Y)
	}

	UE_LOG(Movement_Log, Log, TEXT("AMyCharacter::MoveForwardAndRight-> Function is running"));
}

void AMyCharacter::RotateTowardTarget(float Value)
{
}

void AMyCharacter::StartSprinting()
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::StartSprinting-> Invalid Controller"))
		return;
	}

	GetCharacterMovement()->MaxWalkSpeed=SprintSpeed;

	
}

void AMyCharacter::StopSprinting()
{
	if (!Controller)
	{
		UE_LOG(Movement_Log, Error, TEXT("AMyCharacter::StartSprinting-> Invalid Controller"))
		return;
	}
	
	GetCharacterMovement()->MaxWalkSpeed=MovementSpeed;

}

void AMyCharacter::Dodge(const FInputActionValue& Value)
{
}

void AMyCharacter::DirectionalDodge()
{
}

void AMyCharacter::BackDash()
{
}

void AMyCharacter::TriggerBattleAction()
{
}

void AMyCharacter::SwitchWeapon(const FInputActionValue& Value)
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

void AMyCharacter::SwtichToWeapon()
{
}

void AMyCharacter::LockonTarget()
{
}

