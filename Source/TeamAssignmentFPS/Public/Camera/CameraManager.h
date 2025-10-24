// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManager.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UCameraManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraManagerComp();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "CameraManager")
	bool bIsActive=false;//for default
	
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
	float LocUpdateSpeed;

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


	void AttatchCameraBaseToTarget(USceneComponent* Target);

	bool GetVectorsByCameraAndGravityDirection
	(const FVector& GravityDirection=FVector(0,0,-1), FVector& Forward, FVector& Right, FVector& UpVector);// with default z -1.0
	// this is for providing character the forward and right vector based on the camera rotation and gravity direction
	// gravity direction is neeeded to prevent actor tring to move underground when it trys to go forward

	

		
};
