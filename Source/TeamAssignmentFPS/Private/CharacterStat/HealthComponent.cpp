// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/HealthComponent.h"


UHealthComponent::UHealthComponent():
	MaxHeath(100),
	CurrentHealth(MaxHeath)
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsAlive = CurrentHealth>0;
}

void UHealthComponent::SetMaxHealth(float HP)
{
	MaxHeath = FMath::Max(0,HP);
}

void UHealthComponent::SetCurrentHealth(float HP)
{
	CurrentHealth =  FMath::Clamp(HP,0,MaxHeath);
}

void UHealthComponent::KilledBySettingIsAlive(bool Dead)
{
	bIsAlive = Dead;
	if (!bIsAlive)
	{
		OnDeath.Broadcast(FDamageInfo());	//empty death info. just pure death, no knockback, no elemental damage, just death
	}
}

void UHealthComponent::KilledByDamage(FDamageInfo Damage)
{
	bIsAlive = false;
	OnDeath.Broadcast(Damage);
}

/*void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}*/ //--> maybe for poison damage. or could just use timer handle

void UHealthComponent::GetDamage_Implementation(FDamageInfo Damage)// for now, just the damage amount
{
	CurrentHealth -=Damage.DamageAmount;
	CurrentHealth=FMath::Clamp(CurrentHealth,0,MaxHeath);//clamp the 
	OnHPChanged.Broadcast(CurrentHealth);

	
	 if (OnDamage.IsBound())
	{
		OnDamage.Execute(Damage);//signal to the owner to react based on the damage information
	}


	OnDamage.ExecuteIfBound(Damage);// or use this!!! simple!
	
	if (CurrentHealth<=0 && bIsAlive)
	{
		KilledByDamage(Damage);
	}
}

void UHealthComponent::RecoverHealth_Implementation(float HealAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth+HealAmount,0,MaxHeath);
	OnHPChanged.Broadcast(CurrentHealth);
}
