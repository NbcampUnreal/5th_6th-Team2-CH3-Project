// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InteractionComponent.h"


#include "Debug/UELOGCategories.h"


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

}

void UInteractionComponent::OnInputInteract_Completed(const FInputActionValue& Value)
{
}

void UInteractionComponent::OnInputInteract_Canceled(const FInputActionValue& Value)
{
}

void UInteractionComponent::TriggerInteraction_Tap()
{
}

void UInteractionComponent::TriggerInteraction_HoldStart()
{
}

void UInteractionComponent::TriggerInteraction_HoldUpdate(float UpdateValue)
{
}

void UInteractionComponent::TriggerInteraction_HoldRelease()
{
}


