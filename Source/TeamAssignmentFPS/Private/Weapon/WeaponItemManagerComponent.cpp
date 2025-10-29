// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponItemManagerComponent.h"


// Sets default values for this component's properties
UWeaponItemManagerComponent::UWeaponItemManagerComponent():
	bIsUsingWeapon(true)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UWeaponItemManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	
}


// Called every frame
void UWeaponItemManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UWeaponItemManagerComponent::SwtichWeapon(const FInputActionValue& Value)
{
	
}

void UWeaponItemManagerComponent::SelectItem_PC(const FInputActionValue& Value)
{
}

void UWeaponItemManagerComponent::SelectItem_GP(const FInputActionValue& Value)
{
}

void UWeaponItemManagerComponent::TriggerWeaponOrItem_Press(const FInputActionValue& Value)
{
	if (CurrentWeapon && CurrentWeapon->Implements<UInputReactionInterface>())
	{
		IInputReactionInterface::Execute_OnInputPressed(CurrentWeapon);
	}

	if (CurrentItem && CurrentItem->Implements<UInputReactionInterface>())
	{
		IInputReactionInterface::Execute_OnInputPressed(CurrentItem);
	}
}

void UWeaponItemManagerComponent::TriggerWeaponOrItem_Tap(const FInputActionValue& Value)
{
}

void UWeaponItemManagerComponent::TriggerWeaponOrItem_HoldStart(const FInputActionValue& Value)
{
}

void UWeaponItemManagerComponent::TriggerWeaponOrItem_HoldUpdate(const FInputActionValue& Value)
{
}

void UWeaponItemManagerComponent::TriggerWeaponOrItem_Release(const FInputActionValue& Value)
{
	
}

