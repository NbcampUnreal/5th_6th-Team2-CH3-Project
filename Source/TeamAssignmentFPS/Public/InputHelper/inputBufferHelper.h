// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Delegates/Delegate.h"
#include "inputBufferHelper.generated.h"

/*
	this is for saving input action into a buffer and re call it after the action is over.
	more smooth and natural control
 */

UCLASS()
class TEAMASSIGNMENTFPS_API UinputBufferHelperBase : public UObject
{
	GENERATED_BODY()
protected:
	float BufferDuration=0.3;// for default
	bool bHasBufferedInput=false;
	float BufferStartTime=0.0;

	FTimerHandle BufferTimer;

public:
	virtual void UpdateBuffer(float DeltaTime);
	virtual void ClearBuffer();//remove the buffer
};

template<typename T_Param>// use template so that it can use the param of choice
class TInputBuffer : public UinputBufferHelperBase
{
public:
	DECLARE_DELEGATE_OneParam(FBufferedInputDelegate, T_Param);

	TInputBuffer(){}

	FBufferedInputDelegate BufferedAction;
	T_Param BufferedValue;// to pass on to the triggerd function

	void TryBufferOrExecute
	(const FBufferedInputDelegate BufferedInputDelegate, const T_Param& InputValue, bool bCanExecuteNow/* can it execute now, or not*/)
	{
		if (bCanExecuteNow)// right now!
		{
			BufferedInputDelegate.ExecuteIfBound(InputValue);
		}
		else
		{
			BufferedAction=BufferedInputDelegate;
			BufferedValue=InputValue;
			bHasBufferedInput=true;
			BufferStartTime=0.f;// reset here
		}
	}

	void TryTriggerBufferedInput(bool bCanExecuteNow)
	{
		if (!bHasBufferedInput )
		{
			//invalid
			return;
		}
		if (bCanExecuteNow)
		{
			BufferedAction.ExecuteIfBound(BufferedValue);
		}
	}

	virtual void ClearBuffer()override
	{
		bHasBufferedInput=false;
		BufferedAction.Unbind();
		BufferStartTime=0.f;
	}
};
