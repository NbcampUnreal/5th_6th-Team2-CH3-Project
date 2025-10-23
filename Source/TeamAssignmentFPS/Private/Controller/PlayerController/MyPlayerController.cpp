// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerController/MyPlayerController.h"
#include "Debug/UELOGCategories.h"
#include "Camera/CameraManager.h"
#include "Controller/IMC/IMCManagerComponent.h"



 AMyPlayerController::AMyPlayerController():
	//---- Manager Components ----//
	CameraManagerComp(nullptr),
	IMCManagerComp(nullptr),

	//----Control----//
	bIsGamePad(false),
	bIsInMenu(true)// start from the game menu so true, it is menu
 {
	 
 }


