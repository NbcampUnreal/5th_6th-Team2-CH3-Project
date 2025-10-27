// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/IMC/IMCManagerComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Controller/PlayerController/MyPlayerController.h"
#include "Debug/UELOGCategories.h"

UIMCManagerComp::UIMCManagerComp():
	Subsystem(nullptr),
	EnhancedInputComp(nullptr),
	CurrentPlayerController(nullptr)

{
	PrimaryComponentTick.bCanEverTick = false;


}


// Called when the game starts
void UIMCManagerComp::BeginPlay()
{
	Super::BeginPlay();

	GetSubsystemFromOwnerController();
	
}

void UIMCManagerComp::GetSubsystemFromOwnerController()
{
	AActor* Owner=GetOwner();
	if (!Owner)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Invalid Owner"));
		return;
	}

	CurrentPlayerController = Cast<AMyPlayerController>(Owner->GetInstigatorController());//GetInstigatorController()-->
	if (!CurrentPlayerController)
	{
		CurrentPlayerController=Cast<AMyPlayerController>(Owner);
	}

	if (!CurrentPlayerController)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Cannot find Current controller"));
		return;
	}

	ULocalPlayer* LocalPlayer=CurrentPlayerController->GetLocalPlayer();
	if (!LocalPlayer)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Cannot find valid LocalPlayer"));
		return;
	}

	Subsystem=LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Invalid Subsystem"));
		return;
	}

	EnhancedInputComp=Cast<UEnhancedInputComponent>(CurrentPlayerController->InputComponent);
	if (!EnhancedInputComp)
	{
		UE_LOG(IMC_Log,Error,TEXT("FIMC_Bundle::BindActions->Invalid EnhancedInputComp"));
		return;
	}

	UE_LOG(IMC_Log,Log,TEXT("FIMC_Bundle::BindActions->Setting all done"));
}

void UIMCManagerComp::AddMappingContext(const FIMC_Bundle& IMCB)
{
	if (!IsValidIMC(IMCB,"UIMCManagerComponent::AddMappingContext")) return;

	Subsystem->AddMappingContext(IMCB.GP_IMC.IMC, IMCB.LayerPriority);
	Subsystem->AddMappingContext(IMCB.PC_IMC.IMC, IMCB.LayerPriority);

	UE_LOG(IMC_Log, Log, TEXT("%s->mapping is added"),*IMCB.IMC_B_Name.ToString());
}

void UIMCManagerComp::RemoveMappingContext(const FIMC_Bundle& IMCB)
{
	if (!IsValidIMC(IMCB,"UIMCManagerComponent::RemoveMappingContext")) return;
	
	Subsystem->RemoveMappingContext(IMCB.GP_IMC.IMC);
	Subsystem->RemoveMappingContext(IMCB.PC_IMC.IMC);
	
	UE_LOG(IMC_Log, Log, TEXT("%s->mapping is removed"),*IMCB.IMC_B_Name.ToString());
}

void UIMCManagerComp::BindInputActions(const FIMC_Bundle& IMCB)
{
	if (!IsValidIMC(IMCB,"UIMCManagerComponent::BindInputActions")) return;
	
	BindInputActionByInputType(IMCB,true);
	BindInputActionByInputType(IMCB,false);
	
}

void UIMCManagerComp::BindInputActionByInputType(const FIMC_Bundle& IMCB, bool IsGamePad) const// do that in here now
{
	FIMC_Module SelectedIMCModule=IsGamePad?  IMCB.GP_IMC: IMCB.PC_IMC;
	
	for (FInputActionData& Data: SelectedIMCModule.IA_Data)
	{
		if (!TryBindAction(Data,IMCB)) continue;
		/*if (!Data.InputAction)
		{
			UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::BindInputActionByInputType->invalid InputAction"));
			continue;
		}

		FEnhancedInputActionEventBinding& Binding=
			EnhancedInputComp->BindAction(Data.InputAction, Data.TriggerEvent, IMCB.FunctionOwner, Data.FunctionName);

		if (Binding.GetHandle() ==INDEX_NONE)
		{
			UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::BindInputActionByInputType->Binding failed"));
			continue;
		}
		
		UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::BindInputActionByInputType-> %s binding is done"),*Data.FunctionName.ToString());
		
		Data.SetCurrentIAHandle(Binding.GetHandle());

		Data.SetCurrentIAHandle(Binding.GetHandle());*/
	}
}

void UIMCManagerComp::UnBindInputActions(const FIMC_Bundle& IMCB)
{
	if (!IsValidIMC(IMCB,"UIMCManagerComponent::UnBindInputActions")) return;
	
	UnBindInputActionByInputType(IMCB,true);
	UnBindInputActionByInputType(IMCB,false);

}

void UIMCManagerComp::UnBindInputActionByInputType(const FIMC_Bundle& IMCB, bool IsGamePad) const
{
	FIMC_Module SelectedIMCModule=IsGamePad?  IMCB.GP_IMC: IMCB.PC_IMC;
	
	for (const FInputActionData& Data : SelectedIMCModule.IA_Data)
	{
		if (!Data.InputAction)
		{
			UE_LOG(IMC_Log,Error,TEXT
				("Invalid IA of %s from %s")
				,*Data.FunctionName.ToString(),*IMCB.IMC_B_Name.ToString());
			continue;;
		}
		uint32 HandleID=Data.GetCurrentIAHandle();
		EnhancedInputComp->RemoveBindingByHandle(HandleID);
	}
}

bool UIMCManagerComp::TryBindAction(FInputActionData& Data, const FIMC_Bundle& IMCB) const
{
	if (!EnhancedInputComp)
	{
		UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::TryBindAction -> EnhancedInputComp is nullptr"));
		return false;
	}

	if (!Data.InputAction)
	{
		UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::TryBindAction -> Invalid InputAction for %s"),
			*Data.FunctionName.ToString());
		return false;
	}

	if (!IMCB.FunctionOwner || !IMCB.FunctionOwner->FindFunction(Data.FunctionName))
	{
		FString Owner=IMCB.FunctionOwner?*IMCB.FunctionOwner->GetName() : TEXT("nullptr");
		
		UE_LOG(IMC_Log, Error,
			TEXT("UIMCManagerComp::TryBindAction -> Function not found: %s (Owner: %s)"),
			*Data.FunctionName.ToString(), *Owner);
		return false;
	}

	FEnhancedInputActionEventBinding& Binding =
		EnhancedInputComp->BindAction(Data.InputAction, Data.TriggerEvent, IMCB.FunctionOwner, Data.FunctionName);

	if (Binding.GetHandle() == INDEX_NONE)// binding failed
	{
		UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::TryBindAction -> Failed to bind %s"),
			*Data.FunctionName.ToString());
		return false;
	}

	Data.SetCurrentIAHandle(Binding.GetHandle());
	UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::TryBindAction -> Bound %s successfully (Handle: %d)"),
		*Data.FunctionName.ToString(),Binding.GetHandle());
	return true;
}

void UIMCManagerComp::AddMappingAndBind(const FIMC_Bundle& IMCB)// do both at once
{
	AddMappingContext(IMCB);
	BindInputActions(IMCB);
}

void UIMCManagerComp::ClearIMCB(const FIMC_Bundle& IMCB)
{
	if (!IsValidIMC(IMCB,"UIMCManagerComponent::ClearIMCB")) return;

	UnBindInputActions(IMCB);
	RemoveMappingContext(IMCB);

	UE_LOG(IMC_Log, Log, TEXT("%s is Cleared"), *IMCB.IMC_B_Name.ToString())
}

void UIMCManagerComp::ClearAllIMCB()
{

}

bool UIMCManagerComp::IsValidIMC(const FIMC_Bundle& IMCB, FString DebuggingFunction) const
{
	bool bIsValid=true;

	//------ Imediate return conditions ----//
	if (!Subsystem)
	{
		UE_LOG(IMC_Log,Error,TEXT("%s->Invalid Subsystem"),*DebuggingFunction);
		return false;
	}
	if (!EnhancedInputComp)
	{
		UE_LOG(IMC_Log,Error,TEXT("%s->Invalid EnhancedInputComp"),*DebuggingFunction);
		return false;
	}
	
	//GamePad Checking
	if (!IMCB.GP_IMC.IMC)
	{
		bIsValid=false;
		UE_LOG(IMC_Log,Error,TEXT("%s->Invalid GP_IMC"),*DebuggingFunction);
	}
	if (IMCB.GP_IMC.IA_Data.IsEmpty())
	{
		bIsValid=false;
		UE_LOG(IMC_Log,Error,TEXT("%s->GP IA_data is empty"),*DebuggingFunction);
	}

	//PC Checking
	if (!IMCB.PC_IMC.IMC)
	{
		bIsValid=false;
		UE_LOG(IMC_Log,Error,TEXT("%s->Invalid PC_IMC"),*DebuggingFunction);
	}
	if (IMCB.PC_IMC.IA_Data.IsEmpty())
	{
		bIsValid=false;
		UE_LOG(IMC_Log,Error,TEXT("%s->PC IA_data is empty"),*DebuggingFunction);
	}

	if (bIsValid)
	{
		UE_LOG(IMC_Log,Log,TEXT("%s->is valid"),*DebuggingFunction);
	}
	
	return bIsValid;
}

bool UIMCManagerComp::RegisterIMCB(const FIMC_Bundle& IMCB)
{
	if (IMCB.IMC_B_Name==NAME_None)
	{
		UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::RegisterIMCB -> IMC_Bundle has no valid Name!"));
		return false;
	}

	if (IMCB_Storage.Contains(IMCB.IMC_B_Name))// should it replace or just return? not 
	{
		UE_LOG(IMC_Log, Warning, TEXT("UIMCManagerComp::RegisterIMCB -> IMC_Bundle %s already exists, replacing."),
			*IMCB.IMC_B_Name.ToString());
	}

	IMCB_Storage.Add(IMCB.IMC_B_Name, IMCB);

	UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::RegisterIMCB -> IMC_Bundle %s registered successfully."),
		*IMCB.IMC_B_Name.ToString());
	return true;
}

bool UIMCManagerComp::SetActivationForIMCB(FName ID, bool bIsOn)
{
	FIMC_Bundle* FoundIMCB = IMCB_Storage.Find(ID);
	if (!FoundIMCB)
	{
		UE_LOG(IMC_Log, Warning, TEXT("UIMCManagerComp::SetActivationForIMCB -> IMC %s not found"),
			*ID.ToString());
		return false;
	}

	if (bIsOn)// activate
	{
		if (ActiveIMC_IDs.Contains(ID))
		{
			UE_LOG(IMC_Log, Warning, TEXT("UIMCManagerComp::SetActivationForIMCB -> IMC %s is already active"),
				*ID.ToString());
			return false;
		}

		AddMappingAndBind(*FoundIMCB);
		ActiveIMC_IDs.Add(ID);

		UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::SetActivationForIMCB -> Activated IMC %s"),
			*ID.ToString());
	}
	else// deactivate
	{
		if (!ActiveIMC_IDs.Contains(ID))
		{
			UE_LOG(IMC_Log, Warning, TEXT("UIMCManagerComp::SetActivationForIMCB -> IMC %s is not active"),
				*ID.ToString());
			return false;
		}

		ClearIMCB(*FoundIMCB);
		ActiveIMC_IDs.Remove(ID);

		UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::SetActivationForIMCB -> Deactivated IMC %s"),
			*ID.ToString());
	}

	return true;
}

bool UIMCManagerComp::RemoveIMCB(FName ID)
{
	FIMC_Bundle* FoundIMCB = IMCB_Storage.Find(ID);
	if (!FoundIMCB)
	{
		UE_LOG(IMC_Log, Warning, TEXT("UIMCManagerComp::RemoveIMCB -> IMC %s not found"), *ID.ToString());
		return false;
	}

	// If it’s active, clear and remove from active list
	if (ActiveIMC_IDs.Contains(ID))
	{
		ClearIMCB(*FoundIMCB);
		ActiveIMC_IDs.Remove(ID);
	}

	IMCB_Storage.Remove(ID);
	UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::RemoveIMCB -> IMC %s removed"), *ID.ToString());
	return true;
}
bool UIMCManagerComp::SwapActiveIMCB(FName FromID, FName ToID)
{
	if (FromID == ToID)
	{
		UE_LOG(IMC_Log, Warning, TEXT("UIMCManagerComp::SwapActiveIMCB -> FromID and ToID are the same %s"),
			*ToID.ToString());
		return false;
	}

	FIMC_Bundle* FromIMCB = IMCB_Storage.Find(FromID);
	FIMC_Bundle* ToIMCB = IMCB_Storage.Find(ToID);

	if (!FromIMCB || !ToIMCB)
	{
		UE_LOG(IMC_Log, Error, TEXT("UIMCManagerComp::SwapActiveIMCB -> One or both IMCs not found (From: %s, To: %s)"),
			*FromID.ToString(), *ToID.ToString());
		return false;
	}

	// Only clear if it’s active
	if (ActiveIMC_IDs.Contains(FromID))
	{
		ClearIMCB(*FromIMCB);
		ActiveIMC_IDs.Remove(FromID);
	}

	AddMappingAndBind(*ToIMCB);
	ActiveIMC_IDs.Add(ToID);

	UE_LOG(IMC_Log, Log, TEXT("UIMCManagerComp::SwapActiveIMCB -> Swapped from %s to %s"),
		*FromID.ToString(), *ToID.ToString());
	return true;
}



