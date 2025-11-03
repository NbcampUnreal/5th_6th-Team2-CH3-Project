// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHelper/MouseWheelChunkdetection.h"

void FScrollChunkTracker::ProcessScrollInput(UWorld* World, float ScrollValue)
{
	if (!World || ScrollValue == 0.f)
		return;

	const float Sign = ScrollValue > 0.f ? 1.f : -1.f;

	if (bIsScrolling && Sign != PreviousScrollSign)
	{
		OnChunkEnd.ExecuteIfBound(PreviousScrollSign);
		ResetState();
	}

	if (!bIsScrolling)
	{
		OnChunkStart.ExecuteIfBound(Sign);
		bIsScrolling = true;
		bDidScrollStarted = true;
		PreviousScrollSign = Sign;
	}

	World->GetTimerManager().ClearTimer(ScrollEndTimerHandle);
	World->GetTimerManager().SetTimer(
		ScrollEndTimerHandle,
		FTimerDelegate::CreateLambda([this, Sign]()
		{
			OnChunkEnd.ExecuteIfBound(Sign);
			ResetState();
		}),
		ScrollEndDelay,
		false
	);

	OnChunkStep.ExecuteIfBound(Sign);
}

void FScrollChunkTracker::ResetState()
{
	bIsScrolling = false;
	bDidScrollStarted = false;
	PreviousScrollSign = 0.f;
}