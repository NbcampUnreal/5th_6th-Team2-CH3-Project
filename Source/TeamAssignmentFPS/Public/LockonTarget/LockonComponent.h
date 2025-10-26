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
	AActor* TargetActor=nullptr;// for later possible casting
	
	/*UPROPERTY()
	FVector WorldLocation;*///--> just get it from the Actor
	
	UPROPERTY()
	FVector2D ScreenCoord;// to compare the position on the screen cord
	
	UPROPERTY()
	bool bIsTargeted=false;

};


//forward declaration
//class USphereComponent;// without sphere collision
class AMyPlayerController;
class UCameraManagerComp;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockonComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Debug")
	bool bIsDebugDrawOn=false;
	
protected:
	
	UPROPERTY()
	AMyPlayerController* OwnerController;
	UPROPERTY()
	UCameraManagerComp* CameraManager;
	
	// Confirmation //
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Detection")
	bool bIsActivated=true;

	//bool bIsGamePad=false;// for now, just with the pc controll

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Detection")
	float FalloffRange=100;// auto retargeting condition
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Detection")
	float AutoTargetRadius=200.f;//default radius
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Lockon | Detection")
	USphereComponent* TargetSphere;
	//set sphere on the cusror deprojectec location. the detected targetalbe actors will be traced by this*/
	
	// or need a way to store every visible targetalbes using camera frustum(?) and filter out the targetables which are not in the cursor's range
	//	--> this will be compatable with not only keyboard/mouse, but also with gamepad lock on system
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category="Lockon | Location")
	USceneComponent* LockonBaseRoot=nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category="Lockon | Location")
	AActor* LockonTarget=nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,  Category="Lockon | Location")
	USceneComponent* CameraLandingPlace;//set this on camera location so that camera rig can move to that place

	TArray<FTargetInfo> VisibleTargets;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category="Lockon | Camera")
	float BaseRootToTargetAlpha = 0.5f;//blend alpha value of the camera location

	FVector CursorWorldLocation;// to know where to shoot if it is not on any 
	FVector2D CursorScreenLocation;// the cursor coord on the screen

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
	void UpdateCameraBoomLocation();// middle location by BaseRootToTargetAlpha
	void UpdateCursor();//update cursor Location and projection coord
	void DetectTarget();

	void LockonClosestTargetByCoord();// find nearest Actor to target based on projected 2d screen

	void SwitchTarget(FVector2D SwitchInputDirection);
	// for pc-> when cursor to target distance is too far or other closest targetalbe is found
	// for gamepad -> when target is gone-> find nearest target
	// / or
	// / when target swtich thumbstick is pressed-> find nearest targetable on the direction of the thumbstick

	void CollectCandidatesByCameraView();
	// this will collect the actors on the view port and filterout to find targetables on the screen
public:
	UFUNCTION(BlueprintCallable, Category="Lockon")
	void SetLockonActive(bool bActive){bIsActivated=bActive;}

	UFUNCTION(BlueprintPure, Category="Lockon")
	AActor* GetLockonTarget()const {return LockonTarget;}

	void SetCameraBlendAlpha(float BlendAlpha);
	

		
};
