// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManager.generated.h"

class ACameraRig;

/*USTRUCT(BlueprintType)
struct FCameraStateData// this will hold the camera information to be kept (ex. charcter camera--> location= character, 
{
	GENERATED_BODY()

};*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraManagerComponent();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraManager")
	bool bIsActivated=false;//for default

	//=== Camera Repositioning ====//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	bool bIsTransitioning=false;// for checking if the camera change is completed or not. if completed, this will prevent tick to keep doing transition

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TransitionSpeed=600.f;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")//currently using camera rig
	ACameraRig* CurrentCameraRig=nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")// target rig to be transitioned
	ACameraRig* TargetCameraRig=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TransitionBlendAlpha=0.f;// for transition between rigs

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	

public:	
	//=== Activation ===//--> turn on and off of camera manager
	void ActivateCameraManager();
	void DeactivateCameraManager();

	UFUNCTION(BlueprintCallable, Category = "CameraManager")
	void SetActiveCameraRig(ACameraRig* NewRig);

	UFUNCTION(BlueprintPure, Category = "CameraManager")
	ACameraRig* GetCurrentCameraRig() const {return CurrentCameraRig;}
	UFUNCTION(BlueprintPure, Category = "CameraManager")
	ACameraRig* GetTargetCameraRig() const {return TargetCameraRig;}

	UFUNCTION(BlueprintCallable, Category = "CameraManager")
	void TransitionToTargetRig(ACameraRig* NewRig, float BlendTime = 1.0f);//default time

	UFUNCTION(BlueprintPure, Category = "CameraManager")
	bool IsTransitioning() const { return bIsTransitioning; }

	bool GetVectorsByCameraAndGravityDirection
	(FVector& GravityDirection, FVector& Forward, FVector& Right, FVector& UpVector) const;// with default z -1.0
	// this is for providing character the forward and right vector based on the camera rotation and gravity direction
	// gravity direction is neeeded to prevent actor tring to move underground when it trys to go forward

	

		
};