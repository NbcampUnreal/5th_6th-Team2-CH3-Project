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

//Forward Declares
class UTextBlock;
class UImage;
class UHealthComponent;



UCLASS()
class TEAMASSIGNMENTFPS_API UHealthbar : public UUserWidget
{
	GENERATED_BODY()



protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;//begin play of widget
	virtual void NativeOnInitialized() override;
	
	// this should be for basically creating and initializing hpbar
	
	virtual void NativeDestruct() override;// for destruction
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;//tick for widget
	//basically "Native" is a keyword for c++
	// in this case, lets say native is used in uwidget


	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	bool bIsHealthBarVisible=true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	bool bIsTextVisible=true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Material")
	UImage* HealthBarImage;

	UPROPERTY()
	UTextBlock* HealthBarText;// CurrentHP/MaxHP

	UPROPERTY()
	UTextBlock* DamageText;//Damage amount
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	float DamageDisplayDuration;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	float DamageTextDisappearIntensity;// how quickly 

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Material")
	UMaterialInterface* HealthBarMaterialBase;
	
	UPROPERTY()
	UMaterialInstanceDynamic* HealthBarMID;
	// healthbar needs to show the 

	//==== Health ===//
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "HealthBar | Health")
	UHealthComponent* HPComp;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "HealthBar | Settings")
	float CurrentHealth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	float MaxHealth;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	float SubInterpDelayDuration;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "HealthBar | Settings")
	float SubInterpSpeed;


private:
	//timer to check display duration
	float DamageTimer=0;
	float SubInterpDelayTimer=0;

public:

	void BindToHealthComponent(UHealthComponent* HPComp);
	
	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void SetHPBarMaxHealth(float NewMaxHealth);// this is when the max health is changed

	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void OnHealthDamage(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void OnHealthRecovered(float NewHealth);
	
	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void SetHealthBarVisibility(bool IsVisible);

	UFUNCTION(BlueprintCallable, Category = "HealthBar")
	void ShowDamageText();
	
	
	
};
