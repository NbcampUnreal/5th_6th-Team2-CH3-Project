// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InteractionComponent.h"
#include "InputAction.h"
#include "Character/MyCharacter.h"
#include "Components/SphereComponent.h"

#include "Debug/UELOGCategories.h"
#include "Interface/InputReactionInterface.h"


UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	DetectionSphere=CreateDefaultSubobject<USphereComponent>(TEXT("Detection Sphere"));



}

// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor=Cast<AMyCharacter>(GetOwner());
	if (!OwnerActor)
	{
		UE_LOG(World_Interaction_Log, Error,
			TEXT("UInteractionComponent::BeginPlay -> Owner is not AMyCharacter"));
		return;
	}
	
	SetupInputHandler();
	//Set up for a Detection sphere
	SetupDetectionSphere();
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

void UInteractionComponent::DrawDebugForInteractables()
{
}

bool UInteractionComponent::SetActivationForInteractionComponent(bool bIsActivate)
{
	if (bIsActivated==bIsActivate)// if the setting is same
	{
		UE_LOG(World_Interaction_Log, Warning, TEXT("Already %s")
				,*FString((bIsActivated)?TEXT("Activated"):TEXT("Deactivated")));
		return true;// should it?
	}
	
	if (bIsActivate)
	{
		DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bIsActivated = true;
		UE_LOG(World_Interaction_Log, Log, TEXT("InteractionComponent Activated"));
	}
	else
	{
		DetectionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		InteractableCandidates.Empty();
		CurrentInteractable = nullptr;
		bIsActivated = false;
		UE_LOG(World_Interaction_Log, Log, TEXT("InteractionComponent Deactivated"));
	}
	//deactivate component
	
	return true;
	
}

void UInteractionComponent::OnDetectionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor||OtherActor==GetOwner())
	{
		// invalid actor
		return;
	}

	if (!OtherActor->Implements<UInputReactionInterface>())
	{
		// does not have interface to interact, pass
		return;
	}

	if (InteractableCandidates.Contains(OtherActor))
	{
		//already has it in the other actor
		return;
	}
	
	InteractableCandidates.Add(OtherActor);
	UpdateCurrentInteractable();
	
}

void UInteractionComponent::OnDetectionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		//invalid
		return;
	}

	if (InteractableCandidates.Contains(OtherActor))
	{
		InteractableCandidates.Remove(OtherActor);
		UE_LOG(World_Interaction_Log,Log, TEXT(" UInteractionComponent::OnDetectionSphereEndOverlap-> %s left interaction range."),
			*OtherActor->GetName());
	}

	if (CurrentInteractable==OtherActor)// it the current interactable left the range
	{
		CurrentInteractable=nullptr;// no more current interactable
		UpdateCurrentInteractable();
	}
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

	if (!IsCurrentInteractableValid()) return;

	IInputReactionInterface::Execute_OnInputTap(CurrentInteractable);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_Tap-> Tap"));
	
}

void UInteractionComponent::TriggerInteraction_HoldStart()
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_HoldStart-> try Interaction HoldStart"));

	if (!IsCurrentInteractableValid()) return;

	IInputReactionInterface::Execute_OnInputHoldStart(CurrentInteractable);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_HoldStart-> HoldStart"));
}

void UInteractionComponent::TriggerInteraction_HoldUpdate(float UpdateValue)
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_HoldUpdate-> try Interaction HoldUpdate"));

	if (!IsCurrentInteractableValid()) return;

	IInputReactionInterface::Execute_OnInputHoldUpdate(CurrentInteractable,UpdateValue);
	UE_LOG(World_Interaction_Log, Warning,
		TEXT("UInteractionComponent::TriggerInteraction_HoldUpdate-> HoldUpdate, Value:%f"),UpdateValue);
}

void UInteractionComponent::TriggerInteraction_HoldRelease()
{
	UE_LOG(World_Interaction_Log, Log,
		TEXT("UInteractionComponent::TriggerInteraction_HoldRelease-> try Interaction HoldRelease"));

	if (!IsCurrentInteractableValid()) return;

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

void UInteractionComponent::UpdateCurrentInteractable()
{
	if (InteractableCandidates.IsEmpty())// no candidates at all
	{
		CurrentInteractable=nullptr;
		return;
	}

	FVector OwnerLocation=OwnerActor->GetActorLocation();

	AActor* ClosestInteractable=nullptr;//start from empty
	float ClosestDistance=FLT_MAX;// from the max of float

	for (AActor* CandidateInteractable : InteractableCandidates)
	{
		if (!CandidateInteractable) continue;// invalid-> pass

		float DistanceSquared=FVector::DistSquared(OwnerLocation, CandidateInteractable->GetActorLocation());
		if (DistanceSquared < ClosestDistance)
		{
			ClosestDistance=DistanceSquared;
			ClosestInteractable=CandidateInteractable;
		}
	}

	CurrentInteractable=ClosestInteractable;

	if (!CurrentInteractable)
	{
		UE_LOG(World_Interaction_Log, Log, TEXT("No valid interactable found"));
		return;
	}
	
	UE_LOG(World_Interaction_Log, Log,
		TEXT("Closest Interactable: %s"), *CurrentInteractable->GetName());
}

void UInteractionComponent::SetupDetectionSphere()
{
	if (!OwnerActor)
	{
		UE_LOG(World_Interaction_Log, Error,
			TEXT("UInteractionComponent::SetupDetectionSphere -> Invalid OwnerActor"));
		return;
	}

	if (!DetectionSphere)
	{
		UE_LOG(World_Interaction_Log, Error,
			TEXT("UInteractionComponent::SetupDetectionSphere -> Detection Sphere not created"));
		return;
	}

	//bind Overlap events
	DetectionSphere->OnComponentBeginOverlap.AddDynamic(this,&UInteractionComponent::OnDetectionSphereBeginOverlap );
	DetectionSphere->OnComponentEndOverlap.AddDynamic(this,&UInteractionComponent::OnDetectionSphereEndOverlap);
	
	DetectionSphere->SetupAttachment(OwnerActor->GetRootComponent());
	DetectionSphere->SetSphereRadius(DetectionRadius);
	DetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// no physics needed
	DetectionSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	DetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


