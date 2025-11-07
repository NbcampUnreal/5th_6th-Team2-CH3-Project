// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputHelper/InputActionHandler.h"
#include "InteractionComponent.generated.h"

//forward Declaration
class USphereComponent;// detection area for player character
struct FInputActionValue;
class AMyCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAMASSIGNMENTFPS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();
	
protected:

	// Owner
	UPROPERTY(BlueprintReadOnly, Category="Interaction | Owner")
	AMyCharacter* OwnerActor=nullptr;
	
	//=== Debug Draw ===//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction | Debug")
	bool bDebugOn=false;
	
	//==== Interaction Detection =====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction | Detection")
	USphereComponent* DetectionSphere=nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction | Detection")
	float DetectionRadius=100;//default

	//===== Input Handler ===//

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction | Input")
	UInputActionHandler* InteractionInputHandler=nullptr;

	
	//===== Targets ======//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction | Target")
	AActor* CurrentInteractable=nullptr;// this is currently selected interactable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction | Target")
	TArray<AActor*> InteractableCandidates;// detected interactables around the detection area

private:
	bool bIsActivated=false;
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//=== Debug Draw ===//
	void DrawDebugsForInteractables();
	
public:
	
	//==== Activation =====//

	UFUNCTION(BlueprintCallable, Category = "Interaction | Activation")
	bool SetActivationForInteractionComponent(bool bIsActivate);// toggle by boolean

	//==== Interactable Detection ====//

	UFUNCTION()
	void OnDetectionSphereBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnDetectionSphereEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex);


	
	//==== Input Handler for Interaction =====//
	bool SetupInputHandler();

	UFUNCTION()
	void OnInputInteract_Pressed(const FInputActionValue& Value);// shared by both gmaepad and pc
	UFUNCTION()
	void OnInputInteract_Completed(const FInputActionValue& Value);
	UFUNCTION()
	void OnInputInteract_Canceled(const FInputActionValue& Value);
	
	//==== Interact Button =======//

	void TriggerInteraction_Tap();
	void TriggerInteraction_HoldStart();
	void TriggerInteraction_HoldUpdate(float UpdateValue);
	void TriggerInteraction_HoldRelease();

private:
	bool IsCurrentInteractableValid();
	void UpdateCurrentInteractable();
	void SetupDetectionSphere();
};
	