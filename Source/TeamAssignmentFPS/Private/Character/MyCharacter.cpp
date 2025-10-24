// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "Camera/CameraManager.h"
#include "LockonTarget/LockonComponent.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LockonComp=CreateDefaultSubobject<ULockonComponent>(TEXT("LockonComponent"));
	CameraManagerComp=CreateDefaultSubobject<UCameraManagerComp>(TEXT("CameraManagerComponent"));
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//CameraManagerComp=CreateDefaultSubobject<UCameraManagerComp>(TEXT("LockonComponent"));
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

