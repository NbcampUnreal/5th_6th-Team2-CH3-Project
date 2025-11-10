// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectilePathDrawer.generated.h"

/*
 * // this is for 
 */

UCLASS()
class TEAMASSIGNMENTFPS_API UProjectilePathDrawer : public UObject
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable, Category = "Projectile|Path")
	static FVector GetBezierParabolaPoint(
		const FVector& Start,
		const FVector& End,
		float Scalar01,
		float MinHeight,
		float MaxHeight,
		float T
	);
	


	
};
