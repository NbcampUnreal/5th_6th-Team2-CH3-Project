// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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
class UCameraManagerComponent;
class UHealthComponent;
class UEquipmentManagerComponent;
class UInteractionComponent;
class UInputActionHandler;

//binding function
struct FInputActionValue;

//== Dodge/BackDash
class UCurveFloat;
class UTimelineComponent;

//Movement State Change Update
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMovementStateChanged,
	ECharacterMovementState, PreviousState, ECharacterMovementState, NewState);


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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractionComp")
	UInteractionComponent* InteractionComp;

	//Character Movement State
	UPROPERTY( BlueprintAssignable, Category = "Movement | State")
	FOnMovementStateChanged OnMovementStateChanged;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | State")
	ECharacterMovementState CurrentMovementState;


	// Rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Values")
	float RotationInterpSpeed=5.f;
	
	//==Stat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Values")
	float MovementSpeed=600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement | Values")
	float SprintSpeedMultiplier=1.5;

	bool bIsSprinting=false;
	bool bCanSprint=true;
	
	float CurrentMaxSpeed=600.f;//default

	float MovementSpeedMultiplier=1.f;//default

	FVector2D MovementInputValue;

	//=== Dodge ===//

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement | Dodge")
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
	float DodgeSpeedPlayRate=5.f;
	
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	//Setups
	void SetupForDodgeAction();
	void SetupForInputTypeHelper();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()// must put ufunction for binding fuck
	void MoveForwardAndRight(const FInputActionValue& Value);
	void RotateTowardTarget(float DeltaTime);


	UFUNCTION()
	void TriggerQuickMovement_Pressed(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerQuickMovement_Released(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerQuickMovement_Canceled(const FInputActionValue& Value);

	//Basic movement Setting
	void SetMovementSpeedMultiplier(float MultValue);// this is for debuffing, or buffing the speed by cases
	
	// Sprint
	bool CanCharacterSprint() const {return bCanSprint;}
	void SetCanCharacterSprint(bool CanSprint) {bCanSprint=CanSprint;}
	
	UFUNCTION()
	void StartSprinting();
	UFUNCTION()
	void StopSprinting();

	
	//=== Dodge/BackDash ===//
	UFUNCTION()
	void Dodge();
	void DirectionalDodge();
	void BackDash();
	UFUNCTION()
	void HandleDodgeAction(float DeltaTime);
	UFUNCTION()
	void OnDodgeFinished();


	//Reactions
	UFUNCTION()
	void OnDagamed(FDamageInfo Damage);
	UFUNCTION()
	void OnDeath(FDamageInfo Damage);

	void KockBackCharacter(FDamageInfo Damage);

	//==== Cleanup
	//virtual void AMyCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// == Getter

	ULockonComponent* GetLockonComp() const {return LockonComp;};
private:

	void HealthComponentSetup();
	
	UFUNCTION()
	void HandleOnMovementStateChanged();

	void DecideMovementState();
	
	void SetMovementState(ECharacterMovementState NewState);


};
