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
//binding function
struct FInputActionValue;


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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraComp")
	UCameraManagerComp* CameraManagerComp;


	//==Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MovementSpeed=600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SprintSpeedMultiplier=1.5;

	float CurrentMaxSpeed=600.f;//default

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Movement
	UFUNCTION()// must put ufunction for binding fuck
	void MoveForwardAndRight(const FInputActionValue& Value);

	void RotateTowardTarget(float Value);
	UFUNCTION()
	void StartSprinting();
	UFUNCTION()
	void StopSprinting();
	UFUNCTION()
	void Dodge(const FInputActionValue& Value);
	void DirectionalDodge();
	void BackDash();

	//Attack
	UFUNCTION()
	void TriggerBattleAction();// this can be differ by the current equipped weapon or item(ex. Itme-> use item, weapon-> use weapon)
	UFUNCTION()
	void SwitchAction(const FInputActionValue& Value);// use mouse wheel(pc)or face button?(gamepad)
	//trigger battle action-->
	void UseWeapon();// not so sure should i use interface for both weapon and item?
	void UseItem();// inventory related action
	
	//UseItem
	void SelectItem();// inventory related action
	void SwitchToNewWeapon();// weapon to weapon swtich
	void SwtichToWeapon();// itme to weapon--> same input of switch weapon, but when using item+input-> equip previously used weapon
	
	void LockonTarget();//gamepad only?
};
