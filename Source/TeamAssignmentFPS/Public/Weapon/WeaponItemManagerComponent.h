// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/InputReactionInterface.h"// for weapon triggering
#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"

#include "WeaponItemManagerComponent.generated.h"

class AWeaponBase;
class AItemBase;

class FInputActionValue;//for binding

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UWeaponItemManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponItemManagerComponent();
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	bool bIsUsingWeapon;//weapon or item
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<AWeaponBase> CurrentWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	TObjectPtr<AItemBase> CurrentItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	UObject* CurrentEquipment;// the weapon or item that player character is currently holding

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetCurrentWeapon(AWeaponBase* NewWeapon) {CurrentWeapon = NewWeapon;}
	void SetCurrentItem(AItemBase* NewItem) {CurrentItem = NewItem;}
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION()
	void SwtichWeapon(const FInputActionValue& Value);

	UFUNCTION()
	void SelectItem_PC(const FInputActionValue& Value);//

	UFUNCTION()
	void SelectItem_GP(const FInputActionValue& Value);//

	//---- Weapn and Item Interaction ----//
	UFUNCTION()
	void TriggerWeaponOrItem_Press(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerWeaponOrItem_Tap(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerWeaponOrItem_HoldStart(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerWeaponOrItem_HoldUpdate(const FInputActionValue& Value);
	UFUNCTION()
	void TriggerWeaponOrItem_Release(const FInputActionValue& Value);

private:
	// Helper to call interface on current weapon or item
	template<typename FuncType>
	void CallInterfaceIfValid(FuncType Func)
	{
		UObject* Object= bIsUsingWeapon ? static_cast<UObject*>(CurrentWeapon) : static_cast<UObject*>(CurrentItem);
		
		if (Object && Object->Implements<UInputReactionInterface>())//when the obj is valid and obj has interface
		{
			Func(Object); // Call the lambda or function you passed
		}
	}

};