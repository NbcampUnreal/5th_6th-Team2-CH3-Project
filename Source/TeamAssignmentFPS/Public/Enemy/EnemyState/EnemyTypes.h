// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyTypes.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Spawn UMETA(DisplayName = "Spawn"),
	EES_Idle UMETA(DisplayName = "Idle"),
	EES_Chase UMETA(DisplayName = "Chase"),
	EES_Attack UMETA(DisplayName = "Attack"),
	EES_Damaged UMETA(DisplayName = "Damaged"),
	EES_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	EET_Melee UMETA(DisplayName = "Melee"),
	EET_Range UMETA(DisplayName = "Range"),
	EET_Boss UMETA(DisplayName = "Boss")
};
