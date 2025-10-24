// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class UCameraManagerComp;
class UIMCManagerComp;
class UUIManagerComp;


UCLASS()
class TEAMASSIGNMENTFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AMyPlayerController();

	virtual void BeginPlay() override;

protected:

	//==== ManagerComp ====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	UCameraManagerComp* CameraManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Controller")
	UIMCManagerComp* IMCManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
	UUIManagerComp* UIManager;
	
	//=== Confirmations ===//
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	bool bIsInMenu;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Controller")
	bool bIsGamepad;
	





	
	
public:

	//Manager Comp Getter
	/*UFUNCTION(BlueprintPure, Category = "Manager")
	UCameraManagerComp* GetCameraManager()const {return CameraManager;}*/
	UFUNCTION(BlueprintPure, Category = "Manager")
	UIMCManagerComp* GetIMCManager()const {return IMCManager;}
	UFUNCTION(BlueprintPure, Category = "Manager")
	UUIManagerComp* GetUIManager()const {return UIManager;}

	//----- InputControl ---------//
	//=== InputType Checking ===//
	virtual void SetupInputComponent() override;// set the mapping component
    void OnAnyInputPressed(FKey Key);// check if the input is from pc or gamepad( will return (is Gamepad?))
	void OnMouseAxisMoved(float Value);
	void OnGamePadControllerAxisMoved(float Value);
	
	void SetIsInMenu(bool bIsMenu);
	void SetIsGamePad(bool bIsGamePad);
	
	UFUNCTION(BlueprintPure)
	bool IsUsingGamePad()const {return bIsGamepad;};

	UFUNCTION(BlueprintPure)
	bool IsInMenu()const {return bIsInMenu;};
	//------------------------------------------------------------------//

	//=== 
	
};
