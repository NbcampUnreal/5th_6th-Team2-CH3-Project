// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHelper/InputActionHandler.h"
#include "Debug/UELOGCategories.h"


void UInputActionHandler::SetShouldTriggerWhenCanceled(bool bIsYes)
{
	if (bShouldTriggerWhenCanceled==bIsYes)
	{
		UE_LOG(InputAction_Helper_Log,Warning,
				TEXT("UInputActionHandler::SetbShouldTriggerWhenCanceled-> Already set to %s"),
				*FString((bIsYes?TEXT("YES"):TEXT("NO"))));
		return;
	}
	
	bShouldTriggerWhenCanceled=bIsYes;
	UE_LOG(InputAction_Helper_Log,Log,
				TEXT("UInputActionHandler::SetbShouldTriggerWhenCanceled-> Set to %s"),
				*FString((bIsYes?TEXT("YES"):TEXT("NO"))));
}


void UInputActionHandler::OnTriggerStarted(float Value)
{
	ElapsedTime+=GetWorld()->GetDeltaSeconds();// record times
		
	if (ElapsedTime>TapThresholdTime)// when the holding time has gone beyond threshold time--> Holding
	{
		if (!bIsHolding)// do once
		{
			bIsHolding = true;// change the value to do once
			
			if (!OnHoldStart.IsBound())
			{
				UE_LOG(InputAction_Helper_Log,Log,
					TEXT("UInputActionHandler::OnTriggerStarted OnHoldStart Is not Bound to Function"));
				
				return;
			}
			
			UE_LOG(InputAction_Helper_Log,Log,TEXT("UInputActionHandler::OnTriggerStarted-> HoldStart"));
			OnHoldStart.Execute();// Trigger HoldStart
		}
		else
		{
			if (!OnHoldUpdate_Float.IsBound())
			{
				UE_LOG(InputAction_Helper_Log,Log,
					TEXT("UInputActionHandler::OnTriggerStarted OnHoldUpdate_Float Is not Bound to Function"));
				
				return;
			}
			UE_LOG(InputAction_Helper_Log,Log,
				TEXT("UInputActionHandler::OnTriggerStarted-> Holding. UpdatedValue:%f"), Value);
			OnHoldUpdate_Float.Execute(Value);// update the 
		}
	}
}
void UInputActionHandler::OnTriggerCompleted()
{
	HandleTriggerUpdateEnded();
}

void UInputActionHandler::OnTriggerCanceled()
{
	if (bShouldTriggerWhenCanceled)
	{
		return;
	}
	HandleTriggerUpdateEnded();
}

void UInputActionHandler::HandleTriggerUpdateEnded()
{
	if (ElapsedTime>TapThresholdTime)
	{
		if (!OnReleased.IsBound())//check if it is bound or not
		{
			UE_LOG(InputAction_Helper_Log,Error,
				TEXT("UInputActionHandler::OnTriggerEnded-> OnReleased Not Bound"));
		}
		else
		{
			OnReleased.Execute();// Trigger Released;
			UE_LOG(InputAction_Helper_Log,Log,
				TEXT("UInputActionHandler::OnTriggerEnded-> Hold Released"));
		}
	}
	else
	{
		if (!OnTapped.IsBound())//check if it is bound or not
		{
			UE_LOG(InputAction_Helper_Log,Error,
				TEXT("UInputActionHandler::OnTriggerEnded-> OnTapped Not Bound"));
		}
		else
		{
			OnTapped.Execute();//Trigger Tap
			UE_LOG(InputAction_Helper_Log,Log,
				TEXT("UInputActionHandler::OnTriggerEnded-> Hold Released"));
		}
	}
	
	//reset the checker
	bIsHolding=false;
	ElapsedTime=0.f;
}
