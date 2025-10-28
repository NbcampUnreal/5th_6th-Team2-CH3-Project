// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputTypeReactInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInputTypeReactInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEAMASSIGNMENTFPS_API IInputTypeReactInterface
{
	GENERATED_BODY()
public:
	virtual void OnInputTypeChanged(bool bIsGamepad) =0;//pure
};
