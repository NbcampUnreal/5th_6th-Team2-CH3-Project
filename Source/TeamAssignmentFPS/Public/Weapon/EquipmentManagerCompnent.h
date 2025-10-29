// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponBase.h"
#include "Item/ItemBase.h"

#include "EquipmentManagerCompnent.generated.h"

class AWeaponBase;
class AItemBase;

struct FInputActionValue;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UEquipmentManagerCompnent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerCompnent();
	
protected:
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	bool bIsEquipping;//is current equipment empty or not*///--> no need, just check CurrentEquipment
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	TObjectPtr<AWeaponBase> CurrentWeapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	TObjectPtr<AItemBase> CurrentItem;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	UObject* CurrentEquipment;// the weapon or item that player character is currently holding

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float TapThreshold=0.2f;//default

	//for tracking holding to determine tap or hold
	float CurrentHoldingTime=0.0f;
	bool bDidHoldStarted=false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetCurrentWeapon(AWeaponBase* NewWeapon) {CurrentWeapon = NewWeapon;}
	void SetCurrentItem(AItemBase* NewItem) {CurrentItem = NewItem;}
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*UFUNCTION()
	void BringInventoryComponent();*/
	//--> this is for bringing ptr of inventory component which is in the player character.
	//so that this weapon manager component can access to the inventory and set the current weapon
	//--> or should this be done out side of this comp? not so sure
	//fuck
	
	UFUNCTION()
	void SwtichWeapon(const FInputActionValue& Value);

	UFUNCTION()
	void SelectItem_PC(const FInputActionValue& Value);//

	UFUNCTION()
	void SelectItem_GP(const FInputActionValue& Value);//

	//---- Weapn and Item Interaction ----//

	//Actually binded functions
	
	UFUNCTION()
	void TriggerInput_Start(const FInputActionValue& Value);

	UFUNCTION()
	void TriggerInput_Trigger(const FInputActionValue& Value);

	UFUNCTION()
	void TriggerInput_Complete(const FInputActionValue& Value);

	UFUNCTION()
	void TriggerInput_Ongoing(const FInputActionValue& Value);

	UFUNCTION()
	void TriggerInput_Canceled(const FInputActionValue& Value);

/*private:
	// Helper to call interface on current weapon or item
	template<typename FuncType>
	void TryCallingInterface(FuncType Func)
	{
		if (!CurrentEquipment)
		{
			// no equipment to use
			//TODO:
			// 1. send signal to uimanager to spawn no equipment state widget
			// 2. (error widget), toggle effect on weapon and item slot widget)
			//3. signal character to react(if ther is anim or task for that)
			return;
		}
		
		if (!CurrentEquipment->Implements<UInputReactionInterface>())
		{
			//when equipment does not have InputReactionInterface
			return;
		}
		
		//when the obj is valid and obj has interface
		Func(CurrentEquipment); // Call the lambda or function you passed
	}*///--> this is now done in the #include "Interface/InputReactionInterface.h"

};