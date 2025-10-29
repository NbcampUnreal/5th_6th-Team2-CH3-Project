// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputReactionInterface.generated.h"


/*UENUM(Blueprintable)
enum class ETriggerInputType:uint8
{
	None UMETA(DisplayName = "None"),// error, no enum
	Press UMETA(DisplayName = "Presse"),
	Tap UMETA(DisplayName = "Tap"),
	HoldStart UMETA(DisplayName = "HoldStart"),
	Release UMETA(DisplayName = "Release"),
};*/

UINTERFACE(MinimalAPI)
class UInputReactionInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAMASSIGNMENTFPS_API IInputReactionInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InputTypeTrigger")
	void OnInputPressed();
	virtual void OnInputPressed_Implementation(){}
	//--> default implemenation function so that, when the function is not overwritten, the engine knows that it does not do anything correctly
	//(to prevent crashing)
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InputTypeTrigger")
	void OnInputTap();
	virtual void OnInputTap_Implementation(){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InputTypeTrigger")
	void OnInputHold(float InputValue, bool bIsReleased);// update the float when input type is not just bool
	virtual void OnInputHold_Implementation(float InputValue, bool bIsReleased){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="InputTypeTrigger")
	void OnInputRelease();
	virtual void OnInputRelease_Implementation(){}
};
