// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

//forward Declaration
class USphereComponent;// detection area for player character

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAMASSIGNMENTFPS_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();
	
protected:
	//==== Interaction Detection =====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction | Detection")
	USphereComponent* DetectionSphere;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction | Detection")
	float DetectionRadius=100;//default

	//===== Targets ======//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction | Target")
	AActor* CurrentInteractable;// this is currently selected interactable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction | Target")
	TArray<AActor*> VailableInteractables;// detected interactables around the detection area

private:
	bool bIsActivated=false;
	
protected:
	virtual void BeginPlay() override;
public:
	//==== Activation =====//

	UFUNCTION(BlueprintCallable, Category = "Interaction | Activation")
	bool SetActivationForInteractionComponent(bool bIsActivate);// toggle by boolean
	
	
	//==== Interact Buttom =======//
	UFUNCTION()
	void OnInputInteract_Pressed(const FInputActionValue& Value);// shared by both gmaepad and pc
	UFUNCTION()
	void OnInputInteract_Released(const FInputActionValue& Value);
};
	