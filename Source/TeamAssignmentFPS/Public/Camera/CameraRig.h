// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CameraRig.generated.h"


	/*
	the camera rig is like the name implys, actor that has spring arm for camera boom and camera compoentn to capture the sceene

	Camera manager sets the currently using camera rig to set view target and swap to the next camera rig when it is required

	with this, camera manager can be placed in the controller, and still allows chracter or actor that owns the camera rig to see the view
	in the editor by chaning the settings of camera root location, spring arm legnth and camera filed of view( and many more)
	
	*/

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TEAMASSIGNMENTFPS_API ACameraRig : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraRig();
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera Rig | Activation")
	bool bIsActivated;// is currently using for viewtarget

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera Rig | Activation")
	USceneComponent* CameraBaseRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera Rig | Activation")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Camera Rig | Activation")
	USpringArmComponent* CameraBoom;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetActive(bool bIsActive);

	USceneComponent* GetCameraRoot() const {return CameraBaseRoot;}
	UCameraComponent* GetCamera() const {return CameraComponent;}
	USpringArmComponent* GetSpringArm() const {return CameraBoom;}

	void SetLandingTarget(USceneComponent* NewLandingTarget, float MoveSpeed);
	void MoveToTarget(float DeltaTime);
	void AttatchCameraBaseToTarget(USceneComponent* Target);

};
