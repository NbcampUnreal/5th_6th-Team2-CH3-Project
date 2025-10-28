// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TimeControlHelper.generated.h"

/*
 * this is to controll how the time flows in the game. unreal manage time buy Time(Global world time) and time(Actual time in real world, not from game world)
 *
 * this is for manipulating time flow(pausing, slowing down, speed up)
 */
UCLASS()
class TEAMASSIGNMENTFPS_API UTimeControlHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	static constexpr float MaxDilation=10.0f;// to prevent insanley fast movement that physics could not follow and fails
	//static const float MaxDlation=10.0f; --> Static data member MaxDlation cannot have an in-class initializer
	// use constexpr to define value during complie time so that it can be used in helper class
	
	UFUNCTION(BlueprintPure, Category = "Time")
	static float GetGameTime(UObject* WorldContextObject);
	//WorldContextObject=

	UFUNCTION(BlueprintPure, Category = "Time")
	static float GetRealTime(UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Time")
	static float GetRealDeltaTime();

	UFUNCTION(BlueprintCallable, Category = "Time | FlowControl")
	static void SetGlobalTimeDilation(UObject* WorldContextObject, float Dilation);

	UFUNCTION(BlueprintPure, Category = "Time")
	static float GetGlobalTimeDilation(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category = "Time | FlowControl")
	static void SetActorTimeDilation(AActor* TargetActor, float Dilation);

	UFUNCTION(BlueprintCallable, Category = "Time | FlowControl")
	static void ResetActorTimeDilation(AActor* TargetActor);
	
};


