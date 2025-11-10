// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Parabola_ProjectileBase.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API AParabola_ProjectileBase : public AProjectileBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AParabola_ProjectileBase();
	
protected:
	// Parabola movement variables
	FVector StartLocation;
	FVector TargetLocation;
	float TravelTime;      // Time till it reaches the end
	float ElapsedTime;     // time update

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola")
	float MaxHeight = 200.f;  // arc top height

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola")
	int32 PathSegments = 20;  // For debug

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Parabola")
	FColor PathColor = FColor::Green; // For debug
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateParabolaMovement(float DeltaTime);

	//	Getter
	void GetStartAndEndVector(FVector& Start, FVector& End) const {Start=StartLocation; End=TargetLocation;}
	float GetElapsedTime() const {return ElapsedTime;}

	//	Setter
	void SetStartAndEndLocation(const FVector& Start, const FVector& End) {StartLocation=Start, TargetLocation=End;}
	void SetMaxHeightForParabola(float NewMaxHeight) {MaxHeight = NewMaxHeight;};
	
	virtual void ActivateProjectileBase() override;
	virtual void DeactivateProjectileBase() override;

	
};
