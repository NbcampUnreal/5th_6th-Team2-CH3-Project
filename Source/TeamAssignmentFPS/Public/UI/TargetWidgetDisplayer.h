// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Blueprint/UserWidget.h"

#include "TargetWidgetDisplayer.generated.h"


USTRUCT(BlueprintType)
struct FWidgetStorage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Widget")// use this for the id of the storage
	FName CategoryName= NAME_None;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Widget")
	TMap<FName, TSubclassOf<UUserWidget>> WidgetClasses;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="Widget")// store instanced widget here to manage
	TMap<FName, UUserWidget*> WidgetInstances;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Widget")
	USceneComponent* AnchorPoint=nullptr;// this is for asigning the anchorpoint for widget so that the widget knows where to be spawned and settled
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Widget")
	bool bIsAttatched=false;// is the widget attatched to the anchor or only use the location to spawn
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TEAMASSIGNMENTFPS_API UTargetWidgetDisplayer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetWidgetDisplayer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
public:	

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI | Widget")
	TSubclassOf<UUserWidget> HealthDisplayWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI | Widget")
	TMap<FName, TSubclassOf<UUserWidget>> StateDisplaywidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI | Widget")
	TSubclassOf<UUserWidget> DamageDisplayWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI | Widget")
	TSubclassOf<UUserWidget> LockonIndicatorWidgetClass;*/
		//---> make it more expandalbe

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI | Widget")
	TMap<FName, FWidgetStorage> WidgetLists;

	void CreateFoundWidget(FName StorageName, FName WidgetName);
	void ShowWidget(FName StorageName,  FName WidgetName, int32 Order=0);
	void HideWidget(FName StorageName,  FName WidgetName);
	void RemoveWidget(FName StorageName,  FName WidgetName);

	UFUNCTION(BlueprintCallable, Category = "UI | Widget")
	void SetAnchorForWidgets(FName StorageName, USceneComponent* NewAnchor);// widgets shares the same anchor if they are in same storage

	void UpdateWidgetCoord(FName StorageName);
	void UpdateAllWidgetCoords();
};
