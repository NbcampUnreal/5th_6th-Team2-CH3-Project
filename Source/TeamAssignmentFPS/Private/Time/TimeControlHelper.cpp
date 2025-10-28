// Fill out your copyright notice in the Description page of Project Settings.


#include "Time/TimeControlHelper.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Debug/UELOGCategories.h"

float UTimeControlHelper::GetGameTime(UObject* WorldContextObject)
{
	UWorld*World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(TimeManager_Log,Error, TEXT("UTimeControlHelper::GetGameTime-> World is null"));
			return -1;// for magic number
	}
	return World->GetTimeSeconds();//Affected by global time dilation
}

float UTimeControlHelper::GetRealTime(UObject* WorldContextObject)
{
	UWorld*World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(TimeManager_Log,Error, TEXT("UTimeControlHelper::GetRealTime-> World is null"));
		return -1;// for magic number
	}
	return World->GetRealTimeSeconds();
}

float UTimeControlHelper::GetRealDeltaTime()//simply getting delta time
{
	return FApp::GetDeltaTime();
}

void UTimeControlHelper::SetGlobalTimeDilation(UObject* WorldContextObject, float Dilation)
{
	UWorld*World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(TimeManager_Log,Error, TEXT("UTimeControlHelper::SetGlobalTimeDilation-> World is null"));
		return;
	}

	float SafeDilationValue=FMath::Clamp(Dilation,0.f,MaxDilation);
	
	if (Dilation<0||Dilation>MaxDilation)
	{
		UE_LOG(TimeManager_Log,Warning,TEXT
			("UTimeControlHelper::SetGlobalTimeDilation-> %f is out of range. it is clamped to %f"),Dilation,SafeDilationValue );
	}
	UGameplayStatics::SetGlobalTimeDilation(World, SafeDilationValue);// prevent below 0, and over max dilation
}

float UTimeControlHelper::GetGlobalTimeDilation(UObject* WorldContextObject)
{
	UWorld*World=GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull);
	if (!World)
	{
		UE_LOG(TimeManager_Log,Error, TEXT("UTimeControlHelper::GetGlobalTimeDilation-> World is null"));
		return -1;// error numb
	}

	return World->GetWorldSettings()->GetEffectiveTimeDilation();
}

void UTimeControlHelper::SetActorTimeDilation(AActor* TargetActor, float Dilation)
{
	if (!TargetActor)
	{
		UE_LOG(TimeManager_Log,Error, TEXT("UTimeControlHelper::SetActorTimeDilation-> Invalid TargetActor"));
		return;
	}
	float SafeDilationValue=FMath::Clamp(Dilation,0.f,MaxDilation);
	
	if (Dilation<0||Dilation>MaxDilation)
	{
		UE_LOG(TimeManager_Log,Warning,TEXT
			("UTimeControlHelper::SetActorTimeDilation-> %f is out of range. it is clamped to %f"),Dilation,SafeDilationValue );
	}

	TargetActor->CustomTimeDilation=SafeDilationValue;
}

void UTimeControlHelper::ResetActorTimeDilation(AActor* TargetActor)
{
	if (!TargetActor)
	{
		UE_LOG(TimeManager_Log,Error, TEXT(" UTimeControlHelper::ResetActorTimeDilation-> Invalid TargetActor"));
		return;
	}

	TargetActor->CustomTimeDilation=1.0;
}
