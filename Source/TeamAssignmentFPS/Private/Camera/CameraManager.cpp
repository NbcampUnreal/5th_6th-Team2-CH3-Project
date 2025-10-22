// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraManager.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


UCameraManager::UCameraManager()
{
	PrimaryComponentTick.bCanEverTick = true;

	CameraRoot=CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	
	CameraBoom =CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CameraRoot);
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraRoot);
}


void UCameraManager::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCameraManager::UpdateCameraTransform()
{
	
}

