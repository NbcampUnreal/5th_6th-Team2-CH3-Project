#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerController.generated.h"

class UCameraManagerComp;
class UIMCManagerComp;
class UUIManagerComp;

//DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputTypeChanged, bool /*bIsGamepad*/);

UCLASS()
class TEAMASSIGNMENTFPS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    AMyPlayerController();

	
	//Declare for input update
	//FOnInputTypeChanged OnInputTypeChanged;--> for now, only widget needs the update. so widget has the delegate for now

	
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
	virtual void BeginPlay() override;
	//Manager Comp Getter
	UFUNCTION(BlueprintPure, Category = "Manager")
	UCameraManagerComp* GetCameraManager()const {return CameraManager;}
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
	void SetIsGamePad(bool NewbIsGamePad);
	
	UFUNCTION(BlueprintPure)
	bool IsUsingGamePad()const {return bIsGamepad;};

	UFUNCTION(BlueprintPure)
	bool IsInMenu()const {return bIsInMenu;};
	//------------------------------------------------------------------//

	//===== Activation ====//

	void ActivateCameraManager();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWigetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;

	UFUNCTION(BlueprintCallable, Category = "HUD")
	UUserWidget* GetHUDWidget() const;
};
