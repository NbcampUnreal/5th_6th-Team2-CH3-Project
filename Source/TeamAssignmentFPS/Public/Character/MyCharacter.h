// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LockonTarget/LockonComponent.h"
#include "MyCharacter.generated.h"



UENUM(BlueprintType)// for informing character movement state
enum class ECharacterMovementState:uint8
{
	None UMETA(DisplayName="None"),
	Idle UMETA(DisplayName="Idle"),
	Moving UMETA(DisplayName="Moving"),
	Sprinting UMETA(DisplayName="Sprinting"),
	Dodging UMETA(DisplayName="Dodging"),
	LayingDown UMETA(DisplayName="Crouching"),
	CrouchMoving UMETA(DisplayName="CrouchMoving"),//moving while crouching
	Falling UMETA(DisplayName="Falling")
};

//Forward Declare
class ULockonComponent;// to update the forward rotaion that character needs to update
class UCameraManagerComp;
UCLASS()
class TEAMASSIGNMENTFPS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockonComp")
	ULockonComponent* LockonComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockonComp")
	UCameraManagerComp* CameraManagerComp;

	
	bool bIsLockedOnTarget=false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Movement
	void MoveForwardAndRight(FVector2D Value);

	void RotateTowardTarget(float Value);

	void Dodge(bool Value);
	void DirectionalDodge();
	void BackDash();

	//Attack
	void SwitchWeapon(float Value);
	void UseWeaponOrItem(bool Value);
	
	//UseItem
	void SelectItem();
	void SwtichToWeapon();
};
