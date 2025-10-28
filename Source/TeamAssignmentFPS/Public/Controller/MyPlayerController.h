// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MyPlayerController.generated.h"

class UCameraManager;


UCLASS()
class TEAMASSIGNMENTFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AMyPlayerController();

	virtual void BeginPlay() override;

protected:

	//	=====	Control

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* GamePlayIMC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* MenuIMC;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_TriggerWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* IA_OpenMenu;
	
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	bool bIsInMenu;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	UCameraManager* CameraManagerComp;
	
public:
	UFUNCTION()
	void SetupIMC();
	
};
