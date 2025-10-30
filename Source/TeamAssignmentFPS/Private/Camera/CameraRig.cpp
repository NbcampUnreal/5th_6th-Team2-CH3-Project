// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraRig.h"


#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Debug/UELOGCategories.h"

// Sets default values
ACameraRig::ACameraRig()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBaseRoot=CreateDefaultSubobject<USceneComponent>(TEXT("CameraBaseRoot"));
	SetRootComponent(CameraBaseRoot);

	CameraBoom=CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CameraBaseRoot);
	
	CameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom);
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

	if (bIsActivated && bIsMoving)
	{
		MoveToTarget(DeltaTime);
	}
}

void ACameraRig::SetActive(bool bNewIsActive)
{
	if (bIsActivated==bNewIsActive)
	{
		FString Activation=bIsActivated? "Activated":"Deactivated";
		UE_LOG(Camera_Log,Error,TEXT("ACameraRig::SetActive->Already %s"),*Activation);
		return;
	}
	bIsActivated=bNewIsActive;
	
	if (!CameraComponent)
	{
		UE_LOG(Camera_Log,Error,TEXT("ACameraRig::SetActive->Invalid Camera comp"));
		return;
	}
	
	CameraComponent->SetVisibility(bIsActivated);// turn on camera
}

void ACameraRig::SetLandingTarget(USceneComponent* NewLandingTarget, float InMoveSpeed)
{
	if (!NewLandingTarget)
	{
		UE_LOG(Camera_Log,Error,TEXT("ACameraRig::SetLandingTarget->Invalid LandingTarget"));
		return;
	}

	MoveTarget=NewLandingTarget;
	MoveSpeed=FMath::Max(0.f,InMoveSpeed);// no negative// should it also limit max?
	bIsMoving=true;
}

void ACameraRig::MoveToTarget(float DeltaTime)
{
	if (!MoveTarget||!bIsMoving)// if the movetarget is invalid or it is not moving
		return;

	FVector CurrentLocation=GetActorLocation();
	FVector TargetLocation=MoveTarget->GetComponentLocation();

	//Location lerp
	FVector NewLocation=FMath::VInterpTo(CurrentLocation,TargetLocation,DeltaTime,MoveSpeed);
	SetActorLocation(NewLocation);

	//Rotation lerp
	FRotator TargetRotation=UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);
	float RotationSpeed = 2.0;// temp, put it in reflection system
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed));

	if (FVector::DistSquared(NewLocation,TargetLocation)<FMath::Square(1.f))
	{
		bIsMoving=false;

		AttatchCameraBaseToTarget(MoveTarget);//land on new location
		MoveTarget=nullptr;// and then reset to nullptr
		
	}
	
}

void ACameraRig::AttatchCameraBaseToTarget(USceneComponent* Target)
{
	if (!Target||!CameraBaseRoot)
	{
		UE_LOG(Camera_Log, Error,TEXT("ACameraRig::AttatchCameraBaseToTarget->Invalid Target or CameraBaseRoot"));
		return;
	}
	
	CameraBaseRoot->AttachToComponent(Target, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}