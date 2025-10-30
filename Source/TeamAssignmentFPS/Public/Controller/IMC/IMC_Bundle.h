// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInput/Public/InputTriggers.h"
#include "EnhancedInputComponent.h"
#include "IMC_Bundle.generated.h"

class UInputAction;

/*//----- Binding Information -----// for the case when one input action has the multiple inputtype (a-> pressed, tap, hold ...)
USTRUCT(BlueprintType)
struct FInputActionBindData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	FName FunctionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	ETriggerEvent TriggerEvent;
};*///--> replace it with map

//-------- InputAction Data ---------//
USTRUCT(BlueprintType)
struct FInputActionData
{
	GENERATED_BODY()
	
	FInputActionData():
	InputAction(nullptr)//, IAHandleIDs(0)//--> this is private and cannot get acces. need somehitng else//-> now the handle is multiple
	{}// IAHandle(0)--> basically just a empty id, do not use INDEX_NONE = -1
	FInputActionData(FName Name, UInputAction* IA, ETriggerEvent Event):
	InputAction(IA)//,IAHandleIDs(0)//--> same
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	TObjectPtr<UInputAction> InputAction;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	TArray<FInputActionBindData> BindVariants;
	*/// basically just lower version of map using struct as pair
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | IA")
	TMap<FName/*Function Name*/,ETriggerEvent/* Trigger event*/> BindActions;// for multiple bindings 
	
private:
	TMap<FName, TArray<uint32>> IAHandleIDs;//for multiple triggers are for same function

public:
	TMap<FName, TArray<uint32>> GetCurrentIAHandles()const{return IAHandleIDs;}

	const TArray<uint32>* GetHandlesByFunction(FName WantedFunctionName)const
	{
		return IAHandleIDs.Find(WantedFunctionName);//??? why should it be const for return?
	}
	
	void AddBindingHandle(FName FunctionName, uint32 Hanlde)
	{
		if (IAHandleIDs.Find(FunctionName))// same function found
		{
			IAHandleIDs.Find(FunctionName)->Add(Hanlde);
		}
		else// when function is new
		{
			IAHandleIDs.Add(FunctionName).Add(Hanlde);
		}
	}

	void ClearBindingHandles(FName FunctionName)
	{
		IAHandleIDs.Remove(FunctionName);
	}

	void ClearAllBindingHandles()
	{
		IAHandleIDs.Empty();
	}
	
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
	TObjectPtr<UInputMappingContext> IMC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Module")
	TArray<FInputActionData> IA_Data;

};
//--------------------------------------------------------------------------------------------------------------------//
//==== IMCB_Type =====//

//---------------------------------------------------------------------------------------------------------------------//

//=== IMC_B ===//
USTRUCT(BlueprintType)
struct FIMC_Bundle
{
	GENERATED_BODY()
	FIMC_Bundle():
	IMC_B_Name(NAME_None), FunctionOwner(nullptr), LayerPriority(0)
	{}
	FIMC_Bundle(FName Name,  UObject* Owner, FIMC_Module GP, FIMC_Module PC, int32 Priority):
	IMC_B_Name(Name),  FunctionOwner(Owner),GP_IMC(GP), PC_IMC(PC), LayerPriority(Priority)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IMC | Bundle")
	FName IMC_B_Name;
	
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
