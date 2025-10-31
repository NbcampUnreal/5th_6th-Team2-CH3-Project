// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Healthbar.generated.h"

/*
 *	this is the health bar using material for visual update so that unlike uprogressbar, it can add visual effect more freely
 *	+ aslo, secondary interp bar to show the damage amount and enable recovery till the secondary bar reaches the current hp bar(like bloodborne or lies of p)
 *	 damage amount and current hp/max hp is updated on the widget
 */
UCLASS()
class TEAMASSIGNMENTFPS_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UMaterialInstanceDynamic* HealthBarMID;//this will communicate with material for visual update

	
public:
	UFUNCTION(BlueprintCallable, Category = "Healthbar")
	void SetHealth(float NewHealth, float NewMaxHealth);

protected:
	virtual void NativeConstruct() override;
	
};
