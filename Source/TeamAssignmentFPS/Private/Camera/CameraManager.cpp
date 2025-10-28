// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraManager.h"

#include "Camera/CameraComponent.h"
#include "Camera/CameraRig.h" //
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Debug/UELOGCategories.h"


UCameraManagerComp::UCameraManagerComp()
{
	PrimaryComponentTick.bCanEverTick = false;// no need to use tick any more

}

void UCameraManagerComp::BeginPlay()
{
	Super::BeginPlay();
}


void UCameraManagerComp::ActivateCameraManager()
{
	bIsActivated = true;

	UE_LOG(Camera_Log,Log, TEXT("UCameraManagerComp::ActivateCameraManager->CameraManager Activated"))
}

void UCameraManagerComp::DeactivateCameraManager()
{
	bIsActivated = false;
	UE_LOG(Camera_Log, Log, TEXT("UCameraManagerComp::ActivateCameraManagerCamera Manager Deactivated."));
}

void UCameraManagerComp::SetActiveCameraRig(ACameraRig* NewRig)
{
	if (!bIsActivated || !NewRig)
	{
		UE_LOG(Camera_Log, Warning, TEXT("UCameraManagerComp::SetActiveCameraRig-> Manager inactive or invalid rig"));
		return;
	}

	// Get player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::SetActiveCameraRig-> PlayerController not found"));
		return;
	}

	// Immediately set view target
	PC->SetViewTarget(NewRig);

	// Update reference
	CurrentCameraRig = NewRig;

	UE_LOG(Camera_Log, Log, TEXT("UCameraManagerComp::SetActiveCameraRig-> Active CameraRig set: %s"),
		*NewRig->GetCameraRigName().ToString());
}

void UCameraManagerComp::TransitionToTargetRig(ACameraRig* NewRig, float BlendTime)
{
	if (!bIsActivated)
	{
		UE_LOG(Camera_Log, Warning, TEXT("CameraManagerComp::TransitionToTargetRig-> Camera manager is not activated"));
		return;
	}

	if (!NewRig)
	{
		UE_LOG(Camera_Log, Warning, TEXT("CameraManagerComp::TransitionToTargetRig-> Invalid Target Rig"));
		return;
	}

	if (CurrentCameraRig == NewRig)
	{
		UE_LOG(Camera_Log, Log, TEXT("CameraManagerComp::TransitionToTargetRig-> Already using this rig"));
		return;
	}

	// Get player controller
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
	{
		UE_LOG(Camera_Log, Error, TEXT("CameraManagerComp::TransitionToTargetRig-> PlayerController not found"));
		return;
	}

	// Switch viewport to new rig
	PC->SetViewTargetWithBlend(NewRig, BlendTime, EViewTargetBlendFunction::VTBlend_Cubic);

	// Update references
	CurrentCameraRig = NewRig;
	UE_LOG(Camera_Log, Log, TEXT("CameraManagerComp::TransitionToTargetRig-> View target switched to: %s"),
		*NewRig->GetCameraRigName().ToString());
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
		UE_LOG(Camera_Log, Error, TEXT("UCameraManagerComp::GetVectorsByCameraAndGravityDirection->Invalid PlayerCameraManager"));
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
