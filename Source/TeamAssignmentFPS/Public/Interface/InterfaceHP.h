// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/DamageInfo.h"
#include "InterfaceHP.generated.h"


UINTERFACE(MinimalAPI)
class UInterfaceHP : public UInterface
{
	GENERATED_BODY()
};

/*
 * 
 */
class TEAMASSIGNMENTFPS_API IInterfaceHP
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="HP")
	void GetDamage(FDamageInfo Damage);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="HP")
	void RecoverHealth(float HealAmount);
	/*UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="HP")
	float GetCurrentHealth()const;*///--> for component
};
