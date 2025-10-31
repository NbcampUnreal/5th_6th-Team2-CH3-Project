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
	OwnerController(nullptr),
	CameraManager(nullptr)

{
	PrimaryComponentTick.bCanEverTick = true;



}



void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	LockonBaseRoot=GetOwner()->GetRootComponent();//set the base root

	//--> set owner controller
	OwnerController = Cast<AMyPlayerController>(GetOwner()->GetInstigatorController());
	if (!OwnerController)
	{
		UE_LOG(Camera_Log, Error, TEXT("ULockonComponent::BeginPlay-> OwnerController not found"));
	}
	else// when the OwnerController is valid
	{
		// Get Camera Manager from Controller
		CameraManager=OwnerController->GetCameraManager();
		if (!CameraManager)
		{
			UE_LOG(Camera_Log, Error, TEXT("ULockonComponent::BeginPlay -> CameraManager not found in controller"));
		}
	}

	
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

bool ULockonComponent::GetDeprojectedCursorLocation(FVector& SetCursorWorldLocation)
{
	if (!bIsActivated||!OwnerController)
	{
		// no targetting done yet
		return false;
	}

	SetCursorWorldLocation=CursorWorldLocation;
	return true;
}

void ULockonComponent::UpdateCameraBoomLocation()
{
	if (!CameraManager || !LockonBaseRoot)//return; //temp log print --> 
	{
		UE_LOG(LockonTarget_Log, Error, TEXT("ULockonComponent::UpdateCameraBoomLocation-> Invalid Camera manager or lock on base root"));
		return;
	}
	

	FVector TargetLocation = LockonTarget ? LockonTarget->GetActorLocation() : CursorWorldLocation;
	FVector BaseLocation = LockonBaseRoot->GetComponentLocation();
	FVector DesiredLocation = FMath::Lerp(BaseLocation, TargetLocation, BaseRootToTargetAlpha);

	// Use CameraManager to move current camera rig
	if (CameraManager->GetCurrentCameraRig())
	{
		ACameraRig* CameraRig = CameraManager->GetCurrentCameraRig();

		//if (CameraRig)--> TODO-> bIsTransitioning -> stop updating the transition when the transition is over
		//CameraManager->TransitionToTargetRig(CameraRig, 0.f);// this is only for swithcing camera rig
		CameraRig->SetActorLocation(DesiredLocation);
	}

	if (bIsDebugDrawOn)
	{

		// Draw Debug Sphere at where Camera rig base is
		DrawDebugSphere
	(GetWorld(),DesiredLocation,25.f,12,FColor::Blue,false,-1.f);
	}
}

void ULockonComponent::UpdateCursor()
{
	if (!OwnerController || !GetOwner()) return;
	
	if (!OwnerController->GetMousePosition(CursorScreenLocation.X, CursorScreenLocation.Y)) return;
	/*{
		UE_LOG(LogTemp, Warning, TEXT("ULockonComponent::UpdateCursor -> Failed to get mouse position"));
	}*///--> no log for the tick--> too much
	FVector WorldOrigin, WorldDir;
	if (!OwnerController->DeprojectScreenPositionToWorld(CursorScreenLocation.X, CursorScreenLocation.Y, WorldOrigin, WorldDir))
		return;
		/*{
		UE_LOG(LogTemp, Warning, TEXT("ULockonComponent::UpdateCursor -> Failed to deproject screen to world"));
		return;
	}*/
	FHitResult HitResult;
	FVector TraceEnd = WorldOrigin + (WorldDir * 1000000.f);


	FCollisionObjectQueryParams ObjectParams;
	/*ObjectParams.AddObjectTypesToQuery(ECC_EngineTraceChannel1);*/
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner()); // ignore self

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,WorldOrigin,TraceEnd,ECC_Visibility,Params);//only visibility
	/*GetWorld()->LineTraceSingleByObjectType(HitResult,WorldOrigin,TraceEnd,ObjectParams,Params);//*///--> this is only for seting cursor
	// targetting not done like this(2d coord comparison)
	
	if (bHit)
	{
		CursorWorldLocation = HitResult.Location;
	}
	else
	{
		// If no hit, set Base as the location
		if (USceneComponent* RootComp = GetOwner()->GetRootComponent())
		{
			CursorWorldLocation = RootComp->GetComponentLocation();
		}
		else
		{
			CursorWorldLocation = TraceEnd;
		}
	}

	if (OwnerController->ProjectWorldLocationToScreen(CursorWorldLocation, CursorScreenLocation))
	{
		// CursorScreenLocation updated successfully
	}
	
	if (bIsDebugDrawOn)
	{
		DrawDebugSphere
		(GetWorld(),CursorWorldLocation,25.f,12,FColor::Red,false,-1.f);
		// Draw debug sphere at the hit or fallback location
		
	}
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

void ULockonComponent::SetCameraBlendAlpha(float BlendAlpha)
{
	float ClampedAlpha=FMath::Clamp(BlendAlpha,0.f,1.f);
	BaseRootToTargetAlpha=BlendAlpha;
}