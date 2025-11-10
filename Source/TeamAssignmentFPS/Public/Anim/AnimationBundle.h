// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationBundle.generated.h"

USTRUCT(Blueprintable)
struct TEAMASSIGNMENTFPS_API FAnimationPair
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	AActor* AnimPlayer;// the actor that use the anim montage 
	
	TMap<uint8/*Anim ID*/, UAnimMontage/*Anim montage*/> AnimMontageBundle;
	
	// multiple anim montage per owner
	
};

USTRUCT(Blueprintable)
struct TEAMASSIGNMENTFPS_API FAnimationBundle
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	AActor* AnimPlayer;// the actor that use the anim montage 

	TMap<uint8/*Anim ID*/, UAnimMontage/*Anim montage*/> AnimMontageBundle;
	
	// multiple anim montage per owner
	
};
