// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/HealthComponent.h"


UHealthComponent::UHealthComponent()
{

	PrimaryComponentTick.bCanEverTick = true;


}



void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

