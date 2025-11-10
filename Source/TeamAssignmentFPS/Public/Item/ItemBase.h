// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/EquipmentInterface.h"
#include "Interface/InputReactionInterface.h"
#include "ItemBase.generated.h"

USTRUCT()
struct FItemStr
{
	GENERATED_BODY()

	FName ItemName;

	UStaticMeshComponent* StaticMesh;
	
};






class UStaticMeshComponent;
class USkeletalMeshComponent;
class UUserWidget;

UCLASS()
class TEAMASSIGNMENTFPS_API AItemBase : public AActor,
	public IInputReactionInterface,
	public IEquipmentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | Component")
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item | Component")
	USkeletalMeshComponent* SkeletalMesh;

	TSubclassOf<UUserWidget> ItemWidget;// widget for this 

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Input Reaction
	virtual void OnInputPressed_Implementation() override;
	virtual void OnInputTap_Implementation() override;
	virtual void OnInputHoldStart_Implementation() override;
	virtual void OnInputHoldUpdate_Implementation(float InputValue) override;
	virtual void OnInputRelease_Implementation() override;
	//Equipment
	virtual void OnEquipped_Implementation() override;
	virtual void OnUnequipped_Implementation() override;

};
