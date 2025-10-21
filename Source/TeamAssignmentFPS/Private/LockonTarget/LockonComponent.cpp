// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonTarget/LockonComponent.h"
#include "Controller/MyPlayerController.h"

ULockonComponent::ULockonComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}



void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();


	
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
{
void  ULockonComponent::DetectTarget()
{
}
