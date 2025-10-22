// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "LockonComponent.generated.h"


USTRUCT(blueprintable)
struct FTargetInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* TargetActor;// for later possible casting
	
	/*UPROPERTY()
	FVector WorldLocation;*///--> just get it from the Actor
	
	UPROPERTY()
	FVector2D ScreenCoord;// to compare with the 
	
	UPROPERTY()
	bool bIsTargeted;

};








//forward declaration
class USphereComponent;
class AMyPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockonComponent();


protected:
	
	UPROPERTY()
	AMyPlayerController* OwnerController;
	
	// Confirmation //
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Detection")
	bool bIsActivated;

	//bool bIsGamePad=false;// for now, just with the pc controll

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Detection")
	float FalloffRange;// auto retargeting condition
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Detection")
	float AutoTargetRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Lockon | Detection")
	USphereComponent* TargetSphere;// spwan this in the cursor porjection location and auto lock on to the target that collides with
	
	FVector OwnerLocation;// to set attatch for the origin point of the targetting system

	FVector CursorWorldLocation;// to know where to locate sphere collision
	FVector CursorScreenLocation;

//-----------------------Fucntions -----------------------------------------------------------------------------------//
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION()
	void SwitchingTargettingSystem(bool bIsOn){bIsActivated=bIsOn;}

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	void UpdateCameraBoomLocation();
	void UpdateCursorProjection();
	void DetectTarget();

		
};
