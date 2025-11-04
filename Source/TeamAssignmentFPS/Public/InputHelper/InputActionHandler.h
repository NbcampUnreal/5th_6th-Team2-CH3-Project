#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputActionHandler.generated.h"

UENUM()
enum EInputActionReactionType : uint8 // these are for binding without params to pass
{
	None UMETA(DisplayName = "None"),
	Tap UMETA(DisplayName = "Tap"),
	HoldStart UMETA(DisplayName = "HoldStart"),
	HoldUpdate UMETA(DisplayName = "HoldUpdate"),
	HoldRelease UMETA(DisplayName = "HoldRelease")
};

UCLASS(Blueprintable, BlueprintType)
class TEAMASSIGNMENTFPS_API UInputActionHandler : public UObject
{
	GENERATED_BODY()

public:
	//DECLARE_DELEGATE(FOnInputPressed); there is no press, only tap or hold
	DECLARE_DELEGATE(FOnInputTap);
	DECLARE_DELEGATE(FOnInputReleased);
	
	//template<typename T_InputValue>
	DECLARE_DELEGATE_OneParam(FOnInputHoldUpdate, float);
	
	DECLARE_DELEGATE(FOnInputHoldStart);
	DECLARE_DELEGATE(FOnInputDoubleTap);// maybe?
	DECLARE_MULTICAST_DELEGATE(FOnInputHoldEnd);
	
	// === Delegates ===//
	FOnInputTap OnTapped;
	FOnInputHoldStart OnHoldStart;
	FOnInputHoldUpdate OnHoldUpdate_Float;
	FOnInputReleased OnReleased;
	
	// use template to take different param
	//FOnInputHoldUpdate<bool> OnHoldUpdate_Boolean;
	//FOnInputHoldUpdate<float> OnHoldUpdate_Float;
	//FOnInputHoldUpdate<FVector2D> OnHoldUpdate_FVector2D;
	// and maybe more.

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float TapThresholdTime = 0.2f;

private:
	bool bShouldTriggerWhenCanceled=false;// this is for when the hold update got cancled and dont want to trigger the hold releas
	bool bIsHolding = false;
	double ElapsedTime = 0.0;
	
public:
	//=== Activation / Binding ===//
	template<typename TClass>
	bool BindOnAction(TClass* Owner,EInputActionReactionType Binder, void (TClass::*Function)(void))
	{
		if (!Owner) return false;

		switch (Binder)
		{
		case EInputActionReactionType::Tap:
			OnTapped.BindUObject(Owner, Function);
			break;
		case EInputActionReactionType::HoldStart:
			OnHoldStart.BindUObject(Owner, Function);
			break;
		case EInputActionReactionType::HoldRelease:
			OnReleased.BindUObject(Owner, Function);
			break;
			
		default:
			//Error
			return false;
		}
		return true;
	}
	
	template<typename TClass, typename T_ParamValueType>
	bool BindActionWithParam(TClass* Owner, void (TClass::*Function)(T_ParamValueType))
	{
		if (!Owner) return false;

		FOnInputHoldUpdate<T_ParamValueType> OnHoldUpdate;
	}

	

	void SetShouldTriggerWhenCanceled(bool bIsYes);// this is for setting the bool ShouldTriggerWhenCancled

	//=== Trigger Manage ===//
	void OnTriggerStarted(float Value);
	void OnTriggerCompleted();
	void OnTriggerCanceled();
	
private:
	void HandleTriggerUpdateEnded();//for both completed and cancled
	
	/*template<typename T_InputValue>// use template for taking input values
void OnTriggerStarted(T_InputValue Value)
	{
		bIsInputTriggered=true;// its just a bool let it set per tick. not that huge
		ElapsedTime+=GetWorld()->GetTimeSeconds();// record times
		
		if (ElapsedTime>TapThresholdTime)// when the holding time go beyond threshold time--> Holding
		{
			if (!bIsHolding)// do once
			{
				OnHoldStart.Execute();// Trigger HoldStart
				bIsHolding = true;
			}
			else
			{
				OnHoldUpdate_Float.Execute(Value);// update the 
			}
		}
	}*/
	
};