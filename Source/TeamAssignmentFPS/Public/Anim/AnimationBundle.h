// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimationBundle.generated.h"

class UAnimMontage;

USTRUCT(Blueprintable)
struct FActorAnimationBundle
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TWeakObjectPtr<AActor> Actor; // safer than raw pointer

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<UAnimMontage*> AnimMontages;
};

USTRUCT(Blueprintable)
struct FAnimationBundle
{
	GENERATED_BODY()
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FName AnimationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<FActorAnimationBundle> ActorBundles;
};