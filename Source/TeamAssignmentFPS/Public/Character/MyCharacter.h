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
class UEquipmentManagerCompnent;
class UInventoryManagerComponent;

//binding function
struct FInputActionValue;

//== Dodge/BackDash
class UCurveFloat;
class UTimelineComponent;

UCLASS()
class TEAMASSIGNMENTFPS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lockon Comp")
	TObjectPtr<ULockonComponent> LockonComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera Comp")
	TObjectPtr<UCameraManagerComponent> CameraManagerComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentInteraction Comp")
	TObjectPtr<UEquipmentManagerCompnent> EquipmentInteractionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory Comp")
	TObjectPtr<UInventoryManagerComponent> InventoryComp;
	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInput | Dodge")
	bool bIsDodging=false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MovementInput | Dodge")
	UTimelineComponent* DodgeTimeLine;
	// function to bind with the timeline
	/*FOnTimelineFloat ProgressFunction;
	FOnTimelineEvent FinishFunction;*/// no need to store it as varaible, timeline stores the binding 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	TObjectPtr<UCurveFloat> DodgeCurve;//ease in and out

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	float DodgeSpeedPlayrate=1.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementInput | Dodge")
	float DodgeDistance=600;//temp base dodge distance
	

	
	
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
	void StartSprinting();
	UFUNCTION()
	void StopSprinting();

	//=== Dodge/BackDash ===//
	UFUNCTION()
	void Dodge(const FInputActionValue& Value);
	void DirectionalDodge();
	void BackDash();

	void HandleDodgeAction(float DeltaTime);
	void OnDodgeFinished();

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
