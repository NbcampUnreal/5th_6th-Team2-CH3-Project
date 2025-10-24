// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerController/MyPlayerController.h"

//=== Managers ===//
#include "Camera/CameraManager.h"
#include "Controller/IMC/IMCManagerComponent.h"
#include "UI/UIManagerComponent.h"

//=== Debug ===//
#include "Debug/UELOGCategories.h"



 AMyPlayerController::AMyPlayerController():
	//---- Manager Components ----//
	CameraManager(nullptr),
	IMCManager(nullptr),
	UIManager(nullptr),


	//----Control----//
	bIsGamepad(false),
	bIsInMenu(true)// start from the game menu so true, it is menu
 {
 	//CameraManager=CreateDefaultSubobject<UCameraManagerComp>(TEXT("Camera Manager Component"));
 	//IMCManager=CreateDefaultSubobject<UIMCManagerComp>(TEXT("IMC Manager Component"));
 	//UIManager=CreateDefaultSubobject<UUIManagerComp>(TEXT("UI Manager Component"));
 }

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

 	if (InputComponent)//member varaible of parent of this class
 	{
		
 		//this is only for checking which button is pressed. not to pawn function


 		//Key/button input detection
 		InputComponent->BindKey(EKeys::AnyKey, IE_Pressed,this, &AMyPlayerController::OnAnyInputPressed);

 		//mouse axis movement detection
 		InputComponent->BindAxisKey(EKeys::MouseX, this, &AMyPlayerController::OnMouseAxisMoved);
 		InputComponent->BindAxisKey(EKeys::MouseY, this, &AMyPlayerController::OnMouseAxisMoved);

 		//gamepad thumbstick axis movement detection
 		InputComponent->BindAxisKey(EKeys::Gamepad_LeftX, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 		InputComponent->BindAxisKey(EKeys::Gamepad_LeftY, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 		InputComponent->BindAxisKey(EKeys::Gamepad_RightX, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 		InputComponent->BindAxisKey(EKeys::Gamepad_RightY, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 		
 		UE_LOG(IMC_Log,Log,TEXT("AMyPlayerController::SetupInputComponent->Binding OnAnyInputPressed Completed"));
 	}
 	UE_LOG(IMC_Log,Error,TEXT("AMyPlayerController::SetupInputComponent->Invalid IC in SetupInputComponent for Checking GP or PC"));
}

void AMyPlayerController::OnAnyInputPressed(FKey Key)
 {
 	if (Key.IsGamepadKey())//check if the updated key is from pc or gamepad
 	{
 		SetIsGamePad(true);
 	}
 	else// if (Key.IsMouseButton()||Key.IsKeyboardKey()) --> for now, default pc, gamepad
 	{
 		SetIsGamePad(false);
 	}
 }

void AMyPlayerController::OnMouseAxisMoved(float Value)
{
 	if (FMath::Abs(Value)<=KINDA_SMALL_NUMBER) return;// or use custom scalar value for threshold
 	
 	SetIsGamePad(false);
}

void AMyPlayerController::OnGamePadControllerAxisMoved(float Value)
{
 	if (FMath::Abs(Value)<=KINDA_SMALL_NUMBER) return;
 	
 	SetIsGamePad(true);
}


void AMyPlayerController::SetIsInMenu(bool bIsMenu)
{
 	
 	//TODO : Make a function to switch imc for menu control
 	//IMCManager-> change imc to menu or game
}

void AMyPlayerController::SetIsGamePad(bool bIsGamePad)
{
 	if (bIsGamepad==true)
 	{
 		bIsGamepad=true;
 		UE_LOG(IMC_Log,Log,TEXT("GamePad"));
 	}
 	else// not gp -> pc or mouse
 	{
 		bIsGamepad=false;
 		UE_LOG(IMC_Log,Log,TEXT("PC or Mouse"));
 	}


 	
 	//TODO : make un ui widget to change the setting of the ui
 	//UIManager->change ui setting
}

