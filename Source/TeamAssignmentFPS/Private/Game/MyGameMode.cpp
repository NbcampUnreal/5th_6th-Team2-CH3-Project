// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyGameMode.h"

#include "Controller/PlayerController/MyPlayerController.h"
#include "Character/MyCharacter.h"

AMyGameMode::AMyGameMode()
{
	DefaultPawnClass= AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

}