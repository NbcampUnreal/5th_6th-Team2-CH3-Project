// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/Healthbar.h"


void UHealthbar::NativeConstruct()// this is not 
{
	Super::NativeConstruct();
}

void UHealthbar::NativeDestruct()
{
	Super::NativeDestruct();
}

void UHealthbar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHealthbar::BindToHealthComponent(UHealthComponent* NewHPComp)
{
	if (!HPComp)
	{
		//error invalid hp comp
		return;
	}
	
}

void UHealthbar::SetHPBarMaxHealth(float NewMaxHealth)
{
}

void UHealthbar::OnHealthDamage(float NewHealth)
{
}

void UHealthbar::OnHealthRecovered(float NewHealth)
{
}

void UHealthbar::SetHealthBarVisibility(bool IsVisible)
{
}

void UHealthbar::ShowDamageText()
{
}
