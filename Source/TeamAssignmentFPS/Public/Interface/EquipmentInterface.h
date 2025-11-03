// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipmentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *  this is for calling when item is equipped and unequipped
 */
class TEAMASSIGNMENTFPS_API IEquipmentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Equipment Interface")
	void OnEquipped();
	virtual void OnEquipped_Implementation(){}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Equipment Interface")
	void OnUnequipped();
	virtual void OnUnequipped_Implementation(){}
	
};
