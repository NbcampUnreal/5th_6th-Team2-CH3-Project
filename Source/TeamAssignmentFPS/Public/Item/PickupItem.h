// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

// this is not a item, this is the spawned holder that has information of itme to be put on inventory by character

UCLASS()
class TEAMASSIGNMENTFPS_API APickupItem : public AActor
{
	GENERATED_BODY()
	
public:	
	APickupItem();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;// for movement of the spawned item, yup

};
