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


