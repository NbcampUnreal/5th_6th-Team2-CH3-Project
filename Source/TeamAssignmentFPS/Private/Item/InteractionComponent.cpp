// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InteractionComponent.h"
#include "InputAction.h"

#include "Debug/UELOGCategories.h"
#include "Interface/InputReactionInterface.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupInputHandler();
}

void UInteractionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	// remove inputhandler
	if (InteractionInputHandler)
	{
		InteractionInputHandler->RemoveFromRoot();
		InteractionInputHandler=nullptr;
	}
}

bool UInteractionComponent::SetActivationForInteractionComponent(bool bIsActivate)
{
	if (bIsActivated==bIsActivate)// if the setting is same
	{
		UE_LOG(World_Interaction_Log, Warning, TEXT("Already %s")
				,*FString((bIsActivated)?TEXT("Deactivated"):TEXT("Activated")));
		return true;// should it?
	}
	
	if (bIsActivate)//activate the interaction component
	{
		// when currently, it is not activated
		
		/* Do Activation Here!!!!!!*/
		
		return true;
	}
	//deactivate component

	/* Do Deactivation here!!!! */

	bIsActivated=bIsActivate;
	return true;
	
}

bool UInteractionComponent::SetupInputHandler()
{
	//start setup

	InteractionInputHandler=NewObject<UInputActionHandler>(this, UInputActionHandler::StaticClass());
	if (!InteractionInputHandler)
	{
		UE_LOG(World_Interaction_Log, Warning,
			TEXT("UInteractionComponent::SetupInputHandler-> Invalid inputhandler"));
		return false;
	}
	
	InteractionInputHandler->AddToRoot();
	InteractionInputHandler->SetShouldTriggerWhenCanceled(true);

	InteractionInputHandler->OnTapped.BindUObject(this, &UInteractionComponent::TriggerInteraction_Tap);
	InteractionInputHandler->OnHoldStart.BindUObject(this, &UInteractionComponent::TriggerInteraction_HoldStart);
	InteractionInputHandler->OnHoldUpdate_Float.BindUObject(this, &UInteractionComponent::TriggerInteraction_HoldUpdate);
	InteractionInputHandler->OnReleased.BindUObject(this, &UInteractionComponent::TriggerInteraction_HoldRelease);

	return true;
	
}

void UInteractionComponent::OnInputInteract_Pressed(const FInputActionValue& Value)
{
	float FloatValue=Value.Get<float>();
	if (!InteractionInputHandler) return;
	InteractionInputHandler->OnTriggerPressed(FloatValue);
}

void UInteractionComponent::OnInputInteract_Completed(const FInputActionValue& Value)
{
	if (!InteractionInputHandler) return;
	InteractionInputHandler->OnTriggerCompleted();
}

void UInteractionComponent::OnInputInteract_Canceled(const FInputActionValue& Value)
{
	if (!InteractionInputHandler) return;
	InteractionInputHandler->OnTriggerCanceled();
}


void UInteractionComponent::TriggerInteraction_Tap()
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_Tap-> try Interaction Tap"));

	if (!IsCurrentInteractableValid) return;

	IInputReactionInterface::Execute_OnInputTap(CurrentInteractable);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_Tap-> Tap"));
	
}

void UInteractionComponent::TriggerInteraction_HoldStart()
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_HoldStart-> try Interaction HoldStart"));

	if (!IsCurrentInteractableValid) return;

	IInputReactionInterface::Execute_OnInputHoldStart(CurrentInteractable);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_HoldStart-> HoldStart"));
}

void UInteractionComponent::TriggerInteraction_HoldUpdate(float UpdateValue)
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_HoldUpdate-> try Interaction HoldUpdate"));

	if (!IsCurrentInteractableValid) return;

	IInputReactionInterface::Execute_OnInputHoldUpdate(CurrentInteractable,UpdateValue);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_HoldUpdate-> HoldUpdate, Value:%f"),UpdateValue);
}

void UInteractionComponent::TriggerInteraction_HoldRelease()
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_HoldRelease-> try Interaction HoldRelease"));

	if (!IsCurrentInteractableValid) return;

	IInputReactionInterface::Execute_OnInputRelease(CurrentInteractable);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_HoldRelease-> HoldRelease"));
}

bool UInteractionComponent::IsCurrentInteractableValid()
{
	if (!CurrentInteractable)
	{
		UE_LOG(World_Interaction_Log, Error,
			TEXT("UInteractionComponent::IsCurrentInteractableValid-> Invalid Target"));
		return false;
	}
	if (!CurrentInteractable->Implements<UInputReactionInterface>())
	{
		UE_LOG(World_Interaction_Log, Error,
			TEXT("UInteractionComponent::IsCurrentInteractableValid-> Target doesnt have required interface"));
		return false;
	}

	// check all done
	return true;
}


