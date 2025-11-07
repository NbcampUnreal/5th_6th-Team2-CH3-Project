// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InterfaceHP.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeath, FDamageInfo )// trigger this when character dies(ex. shows the death skull head over widget, increment kill count on game state
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChanged, float NewHP);// (ex. health bar)
DECLARE_DELEGATE_OneParam(FOnAttacked, FDamageInfo);//to call damager reaction based on the FDamageInfo
// could there be more?

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UHealthComponent : public UActorComponent, public IInterfaceHP
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHeath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float CurrentHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	bool bIsAlive;

public:	
	// Called every frame
	void SetMaxHealth(float HP);
	float GetMaxHealth()const	{return MaxHeath;}
	
	void SetCurrentHealth(float HP);
	float GetCurrentHealth()const	{return CurrentHealth;}
	
	void KilledBySettingIsAlive(bool Dead);
	void KilledByDamage(FDamageInfo Damage);
	bool CheckIfIsDead()const	{return bIsAlive;};


	FOnHPChanged OnHPChanged;
	FOnDeath OnDeath;
	FOnAttacked OnDamage;
	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetDamage_Implementation(FDamageInfo Damage) override;
	virtual void RecoverHealth_Implementation(float HealAmount) override;
		
};