// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonTarget/LockonComponent.h"

#include "Components/SphereComponent.h"
#include "Controller/PlayerController/MyPlayerController.h"

ULockonComponent::ULockonComponent():
	OwnerController(nullptr),
	bIsActivated(true),FalloffRange(100),AutoTargetRadius(120)
{
	PrimaryComponentTick.bCanEverTick = true;



}



void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	/*OwnerActor->GetActorLocation()*/
	
}


void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void  ULockonComponent::UpdateCameraBoomLocation()
{
}

void  ULockonComponent::UpdateCursorProjection()
{
}

void ULockonComponent::DetectTarget()
{
}
