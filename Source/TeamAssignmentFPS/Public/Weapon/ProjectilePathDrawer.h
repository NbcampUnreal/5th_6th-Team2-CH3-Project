#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProjectilePathDrawer.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API UProjectilePathDrawer : public UObject
{
	GENERATED_BODY()

public:
	// Calculates a parabolic point between Start and End
	static FVector GetLerpedParabolaPoint(
		const FVector& Start,
		const FVector& End,
		float ScalarValue,
		float MinHeight,
		float MaxHeight,
		float LerpAlpha,
		//Default values
		float SlowRange = 0.0f // 0 = no slowdown, 1 = strong slowdown near top
	);

	// Draws a debug parabolic arc
	static void DrawLerpedArc(
		UWorld* World,
		const FVector& Start,
		const FVector& End,
		float Scalar01,
		float MinHeight,
		float MaxHeight,
		//Default values
		int32 Segments = 20,
		FColor Color = FColor::Green,
		float LineThickness = 2.0f,
		float SlowRange = 0.0f
	);
};