// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraManager.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Debug/UELOGCategories.h"
#include "Kismet/GameplayStatics.h"// to get currently using camera as view port
#include "Kismet/KismetMathLibrary.h"


UCameraManagerComp::UCameraManagerComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	CameraRoot=CreateDefaultSubobject<USceneComponent>(TEXT("CameraRoot"));
	
	CameraBoom =CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CameraRoot);
	CameraBoom->bUsePawnControlRotation(false);// no rotation by controller
	// rotation lock
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation(false);// no rotation for camera
}


void UCameraManagerComp::BeginPlay()
{
	Super::BeginPlay();


	// the way display location is made 
	
}


void UCameraManagerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCameraManagerComp::ActivateCameraManager()
{
	
}

void UCameraManagerComp::DeactivateCameraManager()
{
}

void UCameraManagerComp::AttatchCameraBaseToTarget(USceneComponent* Target)
{
}

bool UCameraManagerComp::GetVectorsByCameraAndGravityDirection(const FVector& GravityDirection, FVector& Forward, FVector& Right, FVector& UpVector)
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



