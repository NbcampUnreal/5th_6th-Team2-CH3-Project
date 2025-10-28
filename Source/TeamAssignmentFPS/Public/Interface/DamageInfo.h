// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageInfo.generated.h"

USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* DamageCauser;
	
	float DamageAmount;
	
	FVector DamageDirection;
	
	//buff debuff added later
};