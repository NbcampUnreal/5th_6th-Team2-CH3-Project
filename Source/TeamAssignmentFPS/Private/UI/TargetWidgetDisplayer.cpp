// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TargetWidgetDisplayer.h"

#include "Debug/

// Sets default values for this component's properties
UTargetWidgetDisplayer::UTargetWidgetDisplayer()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UTargetWidgetDisplayer::BeginPlay()
{
	Super::BeginPlay();

}

void UTargetWidgetDisplayer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

