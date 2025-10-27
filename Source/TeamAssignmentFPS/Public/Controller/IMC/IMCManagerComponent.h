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
class TEAMASSIGNMENTFPS_API UIMCManagerComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UIMCManagerComp();

private:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* Subsystem;
	UPROPERTY()
	UEnhancedInputComponent* EnhancedInputComp;
	UPROPERTY()
	AMyPlayerController* CurrentPlayerController;

	UPROPERTY()
	TMap<FName, FIMC_Bundle> IMCB_Storage;

	UPROPERTY()
	TSet<FName> ActiveIMC_IDs;

protected:
	virtual void BeginPlay() override;

	void BindInputActionByInputType(const FIMC_Bundle& IMCB, bool IsGamePad) const;
	void UnBindInputActionByInputType(const FIMC_Bundle& IMCB, bool IsGamePad) const;

	bool TryBindAction(FInputActionData& Data, const FIMC_Bundle& IMCB) const;
	
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
	void AddMappingAndBind(const FIMC_Bundle& IMCB);//do both!

	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void ClearIMCB(const FIMC_Bundle& IMCB);//do both!
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	void ClearAllIMCB();

	// Valic Checker
	bool IsValidIMC(const FIMC_Bundle& IMCB, FString DebuggingFunction) const;

	//== Storage Management ==//
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	bool RegisterIMCB(const FIMC_Bundle& IMCB);
	
	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	bool SetActivationForIMCB(FName ID, bool bIsOn);

	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	bool RemoveIMCB(FName ID);

	UFUNCTION(BlueprintCallable, Category = "IMC | Manager")
	bool SwapActiveIMCB(FName FromID, FName ToID);
	
};