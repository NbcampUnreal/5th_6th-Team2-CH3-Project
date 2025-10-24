// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraRig.h"

// Sets default values
ACameraRig::ACameraRig()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraRig::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraRig::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACameraRig::SetActive(bool bIsActive)
{
}

void ACameraRig::SetLandingTarget(USceneComponent* NewLandingTarget, float MoveSpeed)
{
}

void ACameraRig::MoveToTarget(float DeltaTime)
{
}

void ACameraRig::AttatchCameraBaseToTarget(USceneComponent* Target)
{
}

