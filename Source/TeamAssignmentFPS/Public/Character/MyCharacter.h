// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

//Forward Declare
class ULockonComponent;// to update the forward rotaion that character needs to update

UCLASS()
class TEAMASSIGNMENTFPS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "LockonComp")
	ULockonComponent* LockonComp;

	
	bool bIsLockedOnTarget=false;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
