// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "InputHelper/InputActionHandler.h"
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
class UCameraManagerComponent;
class UHealthComponent;
class UEquipmentManagerComponent;
class UInventoryManagerComponent;
//binding function
struct FInputActionValue;

//== Dodge/BackDash
class UCurveFloat;
class UTimelineComponent;

/*DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementStateChanged, ECharacterMovementState, NewState);*/
// why not using dynamic for this time?
//--> no need

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
	UCameraManagerComponent* CameraManagerComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HealthComp")
	UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentComp")
	UEquipmentManagerComponent* EquipmentInteractionComp;

	//Character Movement State
	ECharacterMovementState CurrentMovementState;
	
	//==Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MovementSpeed=600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float SprintSpeedMultiplier=1.5;

	float CurrentMaxSpeed=600.f;//default

	FVector2D MovementInputValue;

	//=== Dodge ===//

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stat")
	UInputActionHandler* DodgeInputDetectionHelper;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInput | Dodge")
	bool bIsDodging=false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInput | Dodge")
	UTimelineComponent* DodgeTimeline;
	
	// function to bind with the timeline
	/*FOnTimelineFloat ProgressFunction;
	FOnTimelineEvent FinishFunction;*/// no need to store it as varaible, timeline stores the binding 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	UCurveFloat* DodgeCurve;//ease in and out

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	float DodgeSpeedPlayrate=5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	float DodgeDistance=600;//temp base dodge distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	float BackDashDistanceRatio=0.7f;//temp, make it shorter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	float BackDashTimeRatio=1.3f;//temp, but make it faster

private:
	FVector DodgeDirection;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetupForDodgeAction();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//Movement
//State
	void SetMovementState(ECharacterMovementState NewMovementState);
	
	UFUNCTION()// must put ufunction for binding fuck
	void MoveForwardAndRight(const FInputActionValue& Value);

	void RotateTowardTarget(float DeltaTime);
	
	UFUNCTION()
	void StartSprinting(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprinting(const FInputActionValue& Value);

	//=== Dodge/BackDash ===//
	UFUNCTION()
	void Dodge(const FInputActionValue& Value);
	void DirectionalDodge();
	void BackDash();
	UFUNCTION()
	void HandleDodgeAction(float DeltaTime);
	UFUNCTION()
	void OnDodgeFinished();

};
