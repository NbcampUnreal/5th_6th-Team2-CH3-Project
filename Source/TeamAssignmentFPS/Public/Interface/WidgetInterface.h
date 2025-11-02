// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetInterface.generated.h"

UINTERFACE()
class UWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/*
 * this is the interface for widgets mostly for how it shows up and hide( entrance and exit function) 
 */
class TEAMASSIGNMENTFPS_API IWidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="UI Interface")
	void ShowsUpWidget();
	virtual void ShowsUpWidget_Implementation(){}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="UI Interface")
	void HidesUpWidget();
	virtual void HidesUpWidget_Implementation(){}
};
