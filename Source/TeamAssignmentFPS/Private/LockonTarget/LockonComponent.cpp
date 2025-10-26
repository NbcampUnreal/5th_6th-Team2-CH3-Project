// Fill out your copyright notice in the Description page of Project Settings.


#include "LockonTarget/LockonComponent.h"

#include "Components/SphereComponent.h"
#include "Controller/PlayerController/MyPlayerController.h"
#include "DrawDebugHelpers.h"

#include "Camera/CameraManager.h"
#include "Camera/CameraRig.h"

#include "Debug/UELOGCategories.h"
#include "Kismet/GameplayStatics.h"

ULockonComponent::ULockonComponent():
	OwnerController(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;



}



void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerController = Cast<AMyPlayerController>(GetOwner()->GetInstigatorController());
	if (!OwnerController)
	{
		UE_LOG(Camera_Log, Warning, TEXT("ULockonComponent::BeginPlay-> OwnerController not found"));
	}

	LockonBaseRoot=GetOwner()->GetRootComponent();
}


void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsActivated||!OwnerController) return;

	UpdateCursor();
	DetectTarget();
	LockonClosestTargetByCoord();
	UpdateCameraBoomLocation();

}

void ULockonComponent::UpdateCameraBoomLocation()
{
	if (!CameraManager || !LockonBaseRoot) return;

	FVector TargetLocation = LockonTarget ? LockonTarget->GetActorLocation() : CursorWorldLocation;
	FVector BaseLocation = LockonBaseRoot->GetComponentLocation();
	FVector DesiredLocation = FMath::Lerp(BaseLocation, TargetLocation, BaseRootToTargetAlpha);

	// Use CameraManager to move current camera rig
	if (CameraManager->GetCurrentCameraRig())
	{
		CameraManager->TransitionToTargetRig(CameraManager->GetCurrentCameraRig(), 0.f);
		CameraManager->GetCurrentCameraRig()->SetActorLocation(DesiredLocation);
	}
}

void ULockonComponent::UpdateCursor()
{
	OwnerController->GetMousePosition(CursorScreenLocation.X, CursorScreenLocation.Y);
	FVector WorldOrigin, WorldDir;
	OwnerController->DeprojectScreenPositionToWorld(CursorScreenLocation.X, CursorScreenLocation.Y, WorldOrigin, WorldDir);
	CursorWorldLocation = WorldOrigin + WorldDir * 10000.f; //Temp max 
}

void ULockonComponent::DetectTarget()
{
	VisibleTargets.Empty();//reset

	TArray<AActor*> Canditates;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Targetable"), Canditates);
	// needs better way to filter out the actor outside of the camera view
	
	for (AActor* Actor : Canditates)
	{
		if (!Actor) continue;

		FTargetInfo Info;
		Info.TargetActor = Actor;

		FVector2D ScreenPos;
		OwnerController->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenPos);
		Info.ScreenCoord = ScreenPos;

		VisibleTargets.Add(Info);
	}
}

void ULockonComponent::LockonClosestTargetByCoord()
{
	if (VisibleTargets.IsEmpty()) return;

	float MinDistance = FLT_MAX;
	FTargetInfo* ClosestTarget = nullptr;

	for (FTargetInfo& Target : VisibleTargets)
	{
		float Distance = FVector2D::Distance(Target.ScreenCoord, CursorScreenLocation);
		if (Distance < MinDistance && Distance < AutoTargetRadius)
		{
			MinDistance = Distance;
			ClosestTarget = &Target;
		}
	}

	if (ClosestTarget)
	{
		LockonTarget = ClosestTarget->TargetActor;
		ClosestTarget->bIsTargeted = true;
	}
	else
	{
		LockonTarget = nullptr;
	}
}

void ULockonComponent::SwitchTarget(FVector2D SwitchInputDirection)
{
	if (VisibleTargets.Num() == 0 || !LockonTarget) return;

	float MinAngle = 180.f;
	AActor* NextTarget = nullptr;

	FVector2D CurrentScreenPos;
	OwnerController->ProjectWorldLocationToScreen(LockonTarget->GetActorLocation(), CurrentScreenPos);

	for (FTargetInfo& Target : VisibleTargets)
	{
		if (Target.TargetActor == LockonTarget) continue;

		FVector2D Dir = (Target.ScreenCoord - CurrentScreenPos).GetSafeNormal();
		float Angle = FMath::Acos(FVector2D::DotProduct(Dir, SwitchInputDirection));
		if (Angle < MinAngle)
		{
			MinAngle = Angle;
			NextTarget = Target.TargetActor;
		}
	}

	if (NextTarget)
	{
		LockonTarget = NextTarget;
	}
}
