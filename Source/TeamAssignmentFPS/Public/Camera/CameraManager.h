// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraManager.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraManager();

protected:


	UObject* RootOwner;// not the controller, but the parent location of the camera root


	
	//===== Camera ====//


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")// put it and adjust the transform with offset
	USceneComponent* CameraRoot;// base root comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	FVector RootWorldLocation;//where to place the CameraBoom root--> the location will be updated 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float LocUpdateSpeed;


	





	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateCameraTransform();

		
};
