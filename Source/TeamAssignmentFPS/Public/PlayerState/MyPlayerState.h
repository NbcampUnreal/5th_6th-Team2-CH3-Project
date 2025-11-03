// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"


//forward delcare
class UInventoryManagerComponent;


UCLASS()
class TEAMASSIGNMENTFPS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public: 
	AMyPlayerState();

	
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Inventory")
	UInventoryManagerComponent* InventoryManager;


		
protected:
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure, Category="Inventory")
	UInventoryManagerComponent* GetInventoryManager() const {return InventoryManager;};
};
