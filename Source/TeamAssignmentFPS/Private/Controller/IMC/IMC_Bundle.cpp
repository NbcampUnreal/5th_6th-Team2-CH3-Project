// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/IMC/IMC_Bundle.h"
#include "Debug/UELOGCategories.h"

/*void FIMC_Bundle::BindActions(UEnhancedInputComponent* Input_Comp, FIMC_Module NewIMC) const
{
	if (!FunctionOwner)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Invalid FunctionOwner"));
		return;//invalid
	}
	if (!Input_Comp)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Invalid EnhancedInputComp"));
		return;//invalid
	}

	for ( FInputActionData& Data: NewIMC.IA_Data)
		//for (const FInputActionData& Data: NewIMC.IA_Data) --> need to set the handle/ no more read only 
	{
		if (!Data.InputAction)
		{
			UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Invalid InputAction"));
			continue;//invalid
		}

		//Data.IAHandle=Input_Comp->BindAction(Data.InputAction, Data.TriggerEvent, FunctionOwner, Data.FunctionName);
		//Data.IAHandle=static_cast<FInputBindingHandle&>(Binding);
		
		FEnhancedInputActionEventBinding& Binding=Input_Comp->BindAction(Data.InputAction, Data.TriggerEvent, FunctionOwner, Data.FunctionName);
		Data.SetCurrentIAHandle(Binding.GetHandle());
		
		UE_LOG(IMC_Log,Log,TEXT("FIMC_Bundle::BindActions-> %s is binded Successfully"),*Data.FunctionName.ToString());
	}
}

void FIMC_Bundle::BindAllActions(UEnhancedInputComponent* Input_Comp) const
{
	UE_LOG(IMC_Log,Log,TEXT("FIMC_Bundle::BindActions->BindAllActions in IMC_B called"));
	
	BindActions(Input_Comp,GP_IMC);//gamepad
	BindActions(Input_Comp,PC_IMC);//pc
}*/

//---> all done in the imc manager now, the bundle stays as simply a storage
