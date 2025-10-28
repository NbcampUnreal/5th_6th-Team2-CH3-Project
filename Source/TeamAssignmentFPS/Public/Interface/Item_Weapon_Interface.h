// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Item_Weapon_Interface.generated.h"


UENUM(Blueprintable)
enum class ETriggerInputType:uint8
{
	None UMETA(DisplayName = "None"),// error, no enum
	Press UMETA(DisplayName = "Presse"),
	Tap UMETA(DisplayName = "Tap"),
	Hold UMETA(DisplayName = "Hold"),
	HoldAndRelease UMETA(DisplayName = "HoldAndRelease"),
};

UINTERFACE(MinimalAPI)
class UItem_Weapon_Interface : public UInterface
{
	GENERATED_BODY()
};

class TEAMASSIGNMENTFPS_API IItem_Weapon_Interface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Item & Weapon")
	void HandleTriggerInput(ETriggerInputType InputType, bool bIsPressed);
};
