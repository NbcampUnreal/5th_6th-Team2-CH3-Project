// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManager.generated.h"

class USpringArmComponent;
class UCameraComponent;

/*USTRUCT(BlueprintType)
struct FCameraStateData// this will hold the camera information to be kept (ex. charcter camera--> location= character, 
{
	GENERATED_BODY()

};*/

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UCameraManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraManagerComp();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraManager")
	bool bIsActivated=false;//for default

	//=== Camera Repositioning ====//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	bool bIsTransitioning=false;// for checking if the camera change is completed or not. if completed, this will prevent tick to keep doing transition

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float TransitionSpeed=600.f;

	UPROPERTY()
	USceneComponent* LandingTarget = nullptr;// where to put the target during repositioning
	
	
	UPROPERTY()
	UObject* RootOwner;// not the controller, but the parent location of the camera root
	
	//===== Camera ====//
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")// put it and adjust the transform with offset
	USceneComponent* CameraRoot;// base root comp
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector RootWorldLocation;//where to place the CameraBoom root--> the location will be updated */
	//just use cameraroot->getworldlocation()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float LocationUpdateSpeed;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//=== Activation ===//--> turn on and off of camera manager
	void ActivateCameraManager();
	void DeactivateCameraManager();
	
	USceneComponent* GetRootComponent() const {return CameraRoot;}// these will be used to change the params like length, view sight, lag speed etc
	UCameraComponent* GetCameraComponent() const {return Camera;}
	USpringArmComponent* GetSpringArmComponent() const {return CameraBoom;}

	void SetLandingTarget(USceneComponent* NewLandingTarget, float MoveSpeed);
	void MoveToTarget(float DeltaTime);
	void AttatchCameraBaseToTarget(USceneComponent* Target);

	bool GetVectorsByCameraAndGravityDirection
	(FVector& GravityDirection, FVector& Forward, FVector& Right, FVector& UpVector) const;// with default z -1.0
	// this is for providing character the forward and right vector based on the camera rotation and gravity direction
	// gravity direction is neeeded to prevent actor tring to move underground when it trys to go forward

	

		
};
