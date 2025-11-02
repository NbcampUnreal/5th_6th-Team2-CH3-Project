// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// this detects the one action of scroll and trigger the action for once, 

struct FScrollChunkTracker
{
    DECLARE_DELEGATE_OneParam(FOnScrollEvent, float);

    float ScrollEndDelay = 0.2f;
    float PreviousScrollSign = 0.f;
    bool bIsScrolling = false;
    bool bDidScrollStarted = false;
    
    FTimerHandle ScrollEndTimerHandle;

    FOnScrollEvent OnChunkStart;
    FOnScrollEvent OnChunkStep;
    FOnScrollEvent OnChunkEnd;

    void ProcessScrollInput(UWorld* World, float ScrollValue);
    void ResetState();

};