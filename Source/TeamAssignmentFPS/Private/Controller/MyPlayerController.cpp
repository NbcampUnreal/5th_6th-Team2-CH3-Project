// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Debug/UELOGCategories.h"



 AMyPlayerController::AMyPlayerController():
	CameraManagerComp(nullptr),

	//----Control----//
	GamePlayIMC(nullptr),
	MenuIMC(nullptr),
	bIsInMenu(true)// start from the game menu so true, it is menu
 {
	 
 }

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

 	SetupIMC();
}

void AMyPlayerController::SetupIMC()
{
 	ULocalPlayer* LocalP=GetLocalPlayer();
 	if (!LocalP)
 	{
 		UE_LOG(IMC_Log,Error,TEXT("AMyPlayerController::SetupIMC--> Invalid Local Player"));
 		return;
 	}

 	UEnhancedInputLocalPlayerSubsystem* Subsystem= LocalP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
 	if (!Subsystem)
 	{
 		
 	}
 
}
