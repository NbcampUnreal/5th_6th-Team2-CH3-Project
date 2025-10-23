// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MyPlayerController.generated.h"

class UCameraManager;
class UIMCManagerComponent;


UCLASS()
class TEAMASSIGNMENTFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AMyPlayerController();

	virtual void BeginPlay() override;

protected:
	
	//=== Confirmations ===//
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	bool bIsInMenu;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	bool bIsGamePad;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	UCameraManager* CameraManagerComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	UCameraManager* IMCManagerComp;



	
	
public:
	void SetIsInMenu(bool bIsInMenu);
	void SetIsGamePad(bool bIsGamePad);
	
	UFUNCTION(BlueprintPure)
	bool IsUsingGamePad()const {return bIsGamePad;};

	UFUNCTION(BlueprintPure)
	bool IsInMenu()const {return bIsInMenu;};
	
};
