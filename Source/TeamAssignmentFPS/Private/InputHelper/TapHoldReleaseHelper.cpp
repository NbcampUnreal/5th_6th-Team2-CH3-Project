// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHelper/TapHoldReleaseHelper.h"

void UTapHoldReleaseHelper::ProcessInput(UWorld* World, bool bIsPressed)
{
	if (!World)
		return;

	if (bIsPressed)
	{
		if (!bPressed)
		{
			bPressed = true;
			bHoldStarted = false;
			HoldTime = 0.f;
			StartHoldTick(World);
		}
	}
	else if (bPressed)
	{
		bPressed = false;
		StopHoldTick(World);

		if (!bHoldStarted)
			OnTap.ExecuteIfBound();
		else
			OnRelease.ExecuteIfBound();

		bHoldStarted = false;
		HoldTime = 0.f;
	}
}

void UTapHoldReleaseHelper::StartHoldTick(UWorld* World)
{
	if (!World)
		return;

	if (!World->GetTimerManager().IsTimerActive(HoldTickHandle))
	{
		World->GetTimerManager().SetTimer(
			HoldTickHandle,
			this,
			&UTapHoldReleaseHelper::TickHoldUpdate,
			HoldTickInterval,
			true
		);
	}
}
void UTapHoldReleaseHelper::StopHoldTick(UWorld* World)
{
	if (!World)
		return;
	World->GetTimerManager().ClearTimer(HoldTickHandle);
}

void UTapHoldReleaseHelper::TickHoldUpdate()
{
	if (!bPressed)
		return;

	UWorld* World = GetWorld();
	if (!World)
		return;

	const float DeltaSeconds = World->GetDeltaSeconds();
	HoldTime += DeltaSeconds;

	if (!bHoldStarted && HoldTime >= TapThreshold)
	{
		bHoldStarted = true;
		OnHoldStart.ExecuteIfBound();
	}

	if (bHoldStarted)
	{
		OnHoldUpdate.ExecuteIfBound(HoldTime);
	}
}

void UTapHoldReleaseHelper::BeginDestroy()
{
	Super::BeginDestroy();

	if (UWorld* World = GetWorld())
	{
		StopHoldTick(World);
	}
}
