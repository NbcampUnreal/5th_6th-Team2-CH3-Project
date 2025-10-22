// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfaceHP.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	float DamageAmount;
	FVector DamageDirection;
	//buff debuff added later
};

/*
USTRUCT(BlueprintType)
struct FHealInfo
{
	GENERATED_BODY()
	float HealAmount;
	//add additional info here
};
*/ //----> seperate params by heal amount and effect(additional for later)





UINTERFACE(MinimalAPI)
class UInterfaceHP : public UInterface
{
	GENERATED_BODY()
};

/**
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
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="HP")
	float GetCurrentHealth()const;
};
