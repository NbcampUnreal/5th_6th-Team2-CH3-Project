// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Controller/IMC/IMC_Bundle.h"
#include "IMCManagerComponent.generated.h"

// this component only do functions not storing imc data

class AMyPlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAMASSIGNMENTFPS_API UIMCManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UIMCManagerComponent();

private:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComp;
	UPROPERTY()
	AMyPlayerController* CurrentPlayerController;

protected:
	virtual void BeginPlay() override;

	void BindInputActionByInputType(const FIMC_Bundle& IMCB, bool IsGamePad) const;
	void UnBindInputActionByInputType(const FIMC_Bundle& IMCB, bool IsGamePad) const;
	
public:
	
	void GetSubsystemFromOwnerController();
	
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void AddMappingContext(const FIMC_Bundle& IMCB);
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void RemoveMappingContext(const FIMC_Bundle& IMCB);

	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void  BindInputActions(const FIMC_Bundle& IMCB);
	
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void UnBindInputActions(const FIMC_Bundle& IMCB);

	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void AddMappincAndBind(const FIMC_Bundle& IMCB);//do both!

	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void ClearIMCB(const FIMC_Bundle& IMCB);//do both!
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void ClearAllIMCB();

	// Valic Checker
	bool IsValidIMC(const FIMC_Bundle& IMCB, FString DebuggingFunction) const;
};