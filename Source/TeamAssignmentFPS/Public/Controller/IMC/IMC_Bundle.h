// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInput/Public/InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "IMC_Bundle.generated.h"

class UInputAction;

//-------- InputAction Data ---------//
USTRUCT(BlueprintType)
struct FInputActionData
{
	GENERATED_BODY()
	
	FInputActionData():
	FunctionName(NAME_None), InputAction(nullptr), TriggerEvent(),IAHandleID(0)//--> this is private and cannot get acces. need somehitng else
	{}// IAHandle(0)--> basically just a empty id, do not use INDEX_NONE = -1
	FInputActionData(FName Name, UInputAction* IA, ETriggerEvent Event):
	FunctionName(Name), InputAction(IA), TriggerEvent(Event),IAHandleID(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	FName FunctionName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	UInputAction* InputAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	ETriggerEvent TriggerEvent;
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IMC | IA")//--> not so sure why but it is not working with reflection system
	//																			-->//cause it is not in reflection system. it is pure code
	//FInputBindingHandle IAHandle;  // store handle for later usage
private:
	uint32 IAHandleID;

public:
	uint32 GetCurrentIAHandle()const{return IAHandleID;}
	void SetCurrentIAHandle(uint32 Handle){IAHandleID = Handle;}
	
};
//--------------------------------------------------------------------------------------------------------------------//
//===== Modular IMC =====//
class UObject;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FIMC_Module
{
	GENERATED_BODY()

	FIMC_Module():
	IMC(nullptr), IA_Data()
	{}
	
	FIMC_Module(UInputMappingContext* In_IMC, TArray<FInputActionData> In_Actions):
	IMC(In_IMC), IA_Data(In_Actions)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Module")
	UInputMappingContext* IMC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Module")
	TArray<FInputActionData> IA_Data;

};
//--------------------------------------------------------------------------------------------------------------------//
//==== IMCB_Type =====//

UENUM(BlueprintType)
enum class EIMC_B_Type:uint8
{
	None UMETA(DisplayName = "None"),
	CharacterMovement UMETA(DisplayName = "CharacterControl"),
	WorldInteraction UMETA(DisplayName = "WorldInteraction"),
	CameraInteraction UMETA(DisplayName = "CameraInteraction"),
	UIInteraction UMETA(DisplayName = "UIInteraction"),// this is for hud ui interaction while playing as character
	MenuInteraction UMETA(DisplayName = "MenuInteraction"),
	Custom UMETA(DisplayName = "Custom")
};
//---------------------------------------------------------------------------------------------------------------------//

//=== IMC_B ===//
USTRUCT(BlueprintType)
struct FIMC_Bundle
{
	GENERATED_BODY()
	FIMC_Bundle():
	IMC_B_Name(NAME_None), IMC_B_Type(EIMC_B_Type::None), FunctionOwner(nullptr), LayerPriority(0)
	{}
	FIMC_Bundle(FName Name, EIMC_B_Type InType, UObject* Owner, FIMC_Module GP, FIMC_Module PC, int32 Priority):
	IMC_B_Name(Name), IMC_B_Type(InType), FunctionOwner(Owner),GP_IMC(GP), PC_IMC(PC), LayerPriority(Priority)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	FName IMC_B_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	EIMC_B_Type IMC_B_Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	UObject* FunctionOwner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	FIMC_Module GP_IMC;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	FIMC_Module PC_IMC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	int32 LayerPriority;

	//void BindActions(UEnhancedInputComponent* Input_Comp, FIMC_Module NewIMC) const;

	//void BindAllActions(UEnhancedInputComponent* Input_Comp) const;

};
