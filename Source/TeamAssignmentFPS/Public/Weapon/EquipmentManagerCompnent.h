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


UENUM()
enum class EEquipmentType:uint8
{
	None UMETA(DisplayName = "None"),// nothing to hold
	Weapon UMETA(DisplayName="Weapon"),
	Item UMETA(DisplayName="Item"),
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UEquipmentManagerCompnent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerCompnent();
	
protected:


	// confirmations

	EEquipmentType CurrentEquipmentType=EEquipmentType::None;// default = none
	
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon")
	bool bIsEquipping;//is current equipment empty or not*///--> no need, just check CurrentEquipment

	//==== Quick Slot ====//
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSlot|Weapon")
	TMap<uint8/*ID*/,TObjectPtr<AActor>/*Weapon*/> WeaponQuickSlot;
	// the id will be shared with the inventory. the information of the weapon will be aquired from the inventory
	uint8 CurrentWeaponID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuickSlot|Item")
	TMap<uint8/*ID*/,TObjectPtr<AActor>/*Item*/> ItemQuickSlot;
	// the id will be shared with the inventory. the information of the weapon will be aquired from the inventory
	uint8 CurrentItemID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Placement")
	USceneComponent* Placement;// the Scene component of where to attach equipment
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Item")
	AActor* CurrentEquipment;// the weapon or item that player character is currently holding


	//==== MouseWheel Tracking ====//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float TapThreshold=0.2f;//default

	//for tracking holding to determine tap or hold
	float CurrentHoldingTime=0.0f;
	bool bDidHoldStarted=false;

	// mouse wheele update ( to make it only work for once)
private:

	FTimerHandle ScrollEndTimerHandle;// for setting timer
	
	float ScrollEndDelay=0.2f;

	float PreviousScrollSign=0.f;//+ or - or 0
	bool bIsScrolling=false;
	bool bDidScrollStarted=false;

	//Test Temp
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SetPlacementComponent(USceneComponent* NewPlacement);

	// for actually showing up the weapon or the item
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void SpawnCurrentEquipment();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	//void SetPlacementComponent(USceneComponent* NewPlacement);// temporally put on public for testing

	//== for mouse scroll detection
	void OnScrollChunkStart(float ScrollDirection);
	void OnScrollChunkStep(float ScrollDirection);
	void OnScrollChunkEnd(float ScrollDirection);
	
	void ProcessScrollDetection(float ScrollDltaValue, float DeltaTime);
	

public:

	//Temp for controlling weapon without inventory( weapon in the editor )
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void TestEquipWeapon(AActor* SettingWeapon);

	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*UFUNCTION()
	void BringInventoryComponent();*/
	//--> this is for bringing ptr of inventory component which is in the player character.
	//so that this weapon manager component can access to the inventory and set the current weapon
	//--> or should this be done out side of this comp? not so sure
	//fuck
	UFUNCTION(BlueprintCallable, Category="Equipment")
	void UpdatePlacementComponent(USceneComponent* NewPlacement);// this will update where to be attatched
	void SetCurrentEquipmentPlacement();// this will attatch the current weapon or item to the placementcomponent
	
	UFUNCTION()
	void SwtichWeapon_PC(const FInputActionValue& Value);// will be done by mouse wheel scroll(one scroll== one switch)

	UFUNCTION()
	void SwtichWeapon_GP(const FInputActionValue& Value);

	void SetCurrentEquipment(AActor* NewEquipment);

	UFUNCTION()
	void SelectItem_PC(const FInputActionValue& Value);//

	UFUNCTION()
	void SelectItem_GP(const FInputActionValue& Value);//
	


	//---- Weapn and Item Interaction ----//

	//Actually binded functions

	//-- basic input reactions
	UFUNCTION()
	void TriggerInput_Reload(const FInputActionValue& Value);
	
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

	//--- Weapon Interaction

	UFUNCTION()
	void ReloadWeapon(const FInputActionValue& Value);

};