// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/MyPlayerState.h"

#include "Item/InventoryManagerComponent.h"

AMyPlayerState::AMyPlayerState()
{
	InventoryManager=CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("Inventory Manager"));
}

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();
	// not decided what to do here... fuck
}
