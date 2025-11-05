// Fill out your copyright notice in the Description page of Project Settings.


#include "InputHelper/inputBufferHelper.h"

void UinputBufferHelperBase::UpdateBuffer(float DeltaTime)
{
	if (!bHasBufferedInput) return;// no buffer to trigger
	BufferStartTime +=DeltaTime;// timing
	if (BufferStartTime>=BufferDuration)
	{
		ClearBuffer();
	}
	
}

void UinputBufferHelperBase::ClearBuffer()
{
	bHasBufferedInput=false;
	BufferStartTime=0.f;
}
