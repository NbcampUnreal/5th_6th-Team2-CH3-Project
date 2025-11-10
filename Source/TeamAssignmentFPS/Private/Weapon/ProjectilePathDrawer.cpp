// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectilePathDrawer.h"
#include "DrawDebugHelpers.h"

FVector UProjectilePathDrawer::GetLerpedParabolaPoint(
	const FVector& Start,
	const FVector& End,
	float ScalarValue,
	float MinHeight,
	float MaxHeight,
	float LerpAlpha,
	float SlowRange)
{
	ScalarValue = FMath::Clamp(ScalarValue, 0.0f, 1.0f);
	LerpAlpha   = FMath::Clamp(LerpAlpha, 0.0f, 1.0f);
	SlowRange   = FMath::Clamp(SlowRange, 0.0f, 1.0f);

	// Apply slowdown shaping to LerpAlpha
	if (SlowRange > 0.0f)
	{
		// SlowRange adjusts how “wide” the slowdown feels
		// Higher SlowRange => slower, smoother top
		float CurveStrength = FMath::Lerp(1.0f, 3.0f, SlowRange); 
		LerpAlpha = 0.5f - 0.5f * FMath::Cos(LerpAlpha * PI / CurveStrength * 2.0f);
	}

	// Linear horizontal position
	FVector LinearPos = FMath::Lerp(Start, End, LerpAlpha);

	// Arc height (controlled by scalar value)
	float ApexHeight = FMath::Lerp(MinHeight, MaxHeight, ScalarValue);

	// Vertical parabola
	float ParabolaFactor = 1.0f - FMath::Square(2.0f * LerpAlpha - 1.0f);
	float HeightOffset = ApexHeight * ParabolaFactor;

	LinearPos.Z += HeightOffset;
	return LinearPos;
}

void UProjectilePathDrawer::DrawLerpedArc(
	UWorld* World,
	const FVector& Start,
	const FVector& End,
	float Scalar01,
	float MinHeight,
	float MaxHeight,
	int32 Segments,
	FColor Color,
	float LineThickness,
	float SlowRange)
{
	if (!World) return;

	FVector Prev = GetLerpedParabolaPoint(Start, End, Scalar01, MinHeight, MaxHeight, 0.0f, SlowRange);

	for (int32 i = 1; i <= Segments; ++i)
	{
		float T = static_cast<float>(i) / Segments;
		FVector Next = GetLerpedParabolaPoint(Start, End, Scalar01, MinHeight, MaxHeight, T, SlowRange);

		DrawDebugLine(World, Prev, Next, Color, false, -1.0f, 0, LineThickness);
		Prev = Next;
	}
}
