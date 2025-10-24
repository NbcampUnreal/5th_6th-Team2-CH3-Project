// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraManager.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Debug/UELOGCategories.h"
#include "Kismet/GameplayStatics.h"// to get currently using camera as view port
#include "Kismet/KismetMathLibrary.h"


UCameraManagerComp::UCameraManagerComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	CameraRoot=CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraBoom =CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	
	CameraBoom->SetupAttachment(CameraRoot);
	CameraBoom->bUsePawnControlRotation=false;// no rotation by controller
	// rotation lock
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	
	
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation=false;// no rotation for camera
}


void UCameraManagerComp::BeginPlay()
{
	Super::BeginPlay();


	// the way display location is made 
	
}


void UCameraManagerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTransitioning)
	{
		MoveToTarget(DeltaTime);
	}
}

void UCameraManagerComp::ActivateCameraManager()
{
	SetComponentTickEnabled(true);
	bIsActivated=true;
}

void UCameraManagerComp::DeactivateCameraManager()
{
	SetComponentTickEnabled(false);
	bIsActivated=false;
}


void UCameraManagerComp::SetLandingTarget(USceneComponent* NewLandingTarget, float MoveSpeed)
{
	if (!NewLandingTarget)
	{
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::MoveToTarget-> Invalid LandingTarget"));
		return;
	}
	if (!Camera)
	{
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::MoveToTarget-> Invalid Camera"));
		return;
	}

	// detatch camera from root for linear transition( no curved path by spring arm rotation)
	if (Camera->GetAttachParent()==CameraRoot || Camera->GetAttachParent()==CameraBoom)// if camera's parent is camera boom or camera root
	{
		Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);//detach and put it in the world
	}

	//Set Variables for moving
	LandingTarget = NewLandingTarget;
	LocationUpdateSpeed = MoveSpeed;
	bIsTransitioning=true;
}

void UCameraManagerComp::MoveToTarget(float DeltaTime)
{
	if (!bIsTransitioning || !LandingTarget || !CameraRoot)// condition for no movement
		return;

	const FVector TargetLocation=LandingTarget->GetComponentLocation();
	const FVector CurrentLocation=CameraRoot->GetComponentLocation();

	const FVector NewLocation= FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime,LocationUpdateSpeed);
	CameraRoot->SetWorldLocation(NewLocation);

	if (FVector::DistSquared(NewLocation, TargetLocation) <=FMath::Square(1))// acceptable distance to end transition
	{
		CameraRoot->SetWorldLocation(NewLocation);
		bIsTransitioning=false;
		LandingTarget=nullptr;

		if (Camera&&CameraBoom&&Camera->GetAttachParent() !=CameraBoom)
		{
			Camera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepWorldTransform);
		}

		UE_LOG(Camera_Log, Log, TEXT("UCameraManagerComp::MoveToTarget-> Move Update completed"));
	}
}

void UCameraManagerComp::AttatchCameraBaseToTarget(USceneComponent* Target)
{
	if (!Target)
	{
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::AttatchCameraBaseToTarget-> Invalid Target"));
		// for possible time when the target is null--> if there is default scene comp for controller, attatch to that
		return;
	}

	CameraRoot->AttachToComponent(Target, FAttachmentTransformRules::KeepWorldTransform);
}

bool UCameraManagerComp::GetVectorsByCameraAndGravityDirection(FVector& GravityDirection, FVector& Forward, FVector& Right, FVector& UpVector) const
{
	if (!GetWorld())
	{
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::GetVectorsByCameraAndGravityDirection->GetWorld Failed"));
		return false;
	}
	
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	if (!PlayerCameraManager)
	{
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::GetVectorsByCameraAndGravityDirection->Invalid PlayerCameramanager"));
		return false;
	}
	
	//rotate the camera rotaion to match with upvector with -gravity direction
	FRotator CameraRotation = PlayerCameraManager->GetCameraRotation();

	FVector CameraForwardVector=UKismetMathLibrary::GetForwardVector(CameraRotation);
	FVector TargetUpVector=-GravityDirection.GetSafeNormal();

	//match the camera up vector with TargetUpVector by rotating pitch of the camera rotation
	FRotator AlignedCameraRotation = UKismetMathLibrary::MakeRotFromXZ(CameraForwardVector, TargetUpVector);

	Forward=UKismetMathLibrary::GetForwardVector(AlignedCameraRotation);
	Right=UKismetMathLibrary::GetRightVector(AlignedCameraRotation);
	UpVector=UKismetMathLibrary::GetUpVector(AlignedCameraRotation);

	return true;
}



