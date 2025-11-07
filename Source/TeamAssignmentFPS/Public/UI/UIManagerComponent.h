// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "UIManagerComponent.generated.h"

//-------- UI Type -----------//

UENUM(BlueprintType)
enum class EUIMode : uint8
{
	None UMETA(DisplayName = "None"),
	Menu UMETA(DisplayName = "Menu"),// main menu, settings 
	Gameplay UMETA(DisplayName = "Gameplay")// like hud
};
//----- Widget Data -----//
USTRUCT(BlueprintType)
struct FWidgetData
{
	GENERATED_BODY()

	FWidgetData():
	WidgetClass(nullptr),WidgetInstance(nullptr),bIsVisible(false)
	{}
	FWidgetData(TSubclassOf<UUserWidget> Class):	
	WidgetClass(Class),WidgetInstance(nullptr),bIsVisible(false)
	{}
	
	UPROPERTY()
	TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY()
	UUserWidget* WidgetInstance;
	UPROPERTY()
	bool bIsVisible;// to filter out the visibility (be shown in vieport or not)
};
//----- Widget Storage -----//
USTRUCT(BlueprintType)
struct FWidgetDataStorage
{
	GENERATED_BODY()

	FWidgetDataStorage():
	UIMode(EUIMode::None)
	{}
	FWidgetDataStorage(EUIMode Mode):
	UIMode(Mode)
	{}
	
	UPROPERTY()
	EUIMode UIMode;
	UPROPERTY()
	TMap <FName, FWidgetData> StoredWidgets;
};


// === Delegate to signal widgets what type of input is used
DECLARE_MULTICAST_DELEGATE_OneParam(FOnInputTypeChanged, bool/*bIsGamepad*/)

//forward Declare

class AMyPlayerController;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UUIManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUIManagerComp();

	// Delegate
	FOnInputTypeChanged OnInputTypeChanged;// to notify the widgets
	
protected:
	UPROPERTY()
	AMyPlayerController* OwnerController;

	UPROPERTY()
	FWidgetDataStorage HUDWidgets;

	UPROPERTY()
	FWidgetDataStorage MenuWidgets;
	
	UPROPERTY()
	EUIMode CurrentUIMode;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	//=== UI Storage Management ===//
	void AddWidget(EUIMode Mode, FName WidgetName, TSubclassOf<UUserWidget> WidgetClass);
	void RemoveWidget(EUIMode Mode,FName WidgetName);

	void ShowWidget(EUIMode Mode,FName WidgetName, int32 Order=0/*default*/);
	void HideWidget(EUIMode Mode,FName WidgetName);

	void ClearWidgetsByMode(EUIMode Mode);
	void ClearAllWidgets();

	bool GetWidgetStorageByMode(EUIMode Mode, FWidgetDataStorage*& WidgetStorage);
	bool IsWidgetValid(FName WidgetName, TSubclassOf<UUserWidget> WidgetClass, FWidgetDataStorage& WidgetStorage);

	EUIMode GetCurrentUIMode()const {return CurrentUIMode;}
//--------------------------------------------------------------------------------------------------------------------//

	//---- controller activation ----//
	void ActivateUIManager(AMyPlayerController* MyController);
	void DeactivateUIManager();
//--------------------------------------------------------------------------------------------------------------------//	

	//=== UI Display ===//
	void SwitchUIMode(EUIMode Mode);// this will switch the ui displayed on the screen. this will be done when entering and exiting

	//=== UI Update by input
	void NotifyInputTypeChange(bool bIsGamePad) const {OnInputTypeChanged.Broadcast(bIsGamePad);};

private:
	void ApplyInputModeByCurrentUIMode();//sets the imcb by the ui mode

		
};

