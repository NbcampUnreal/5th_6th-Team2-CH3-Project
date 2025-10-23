// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBaseCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnEnemyDead, int Score);

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Chase UMETA(DisplayName = "Chase"),
	Attack UMETA(DisplayName = "Attack"),
	Damaged UMETA(DisplayName = "Damaged"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Melee UMETA(DisplayName = "Melee"),
	Range UMETA(DisplayName = "Range"),
	Boss UMETA(DisplayName = "Boss")
};



UCLASS()
class TEAMASSIGNMENTFPS_API AEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBaseCharacter();

	EEnemyState EnemyState;

	FOnEnemyDead OnEnemyDead;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	/*UPROPERTY(VisibleAnywhere)
	FEnemyStats EnmeyStats;*/
};

