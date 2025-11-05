#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TapHoldReleaseHelper.generated.h"

/*
	this is for quicky triggering the event based on timing the first press and release time
	when it is shorter than holding time --> tap / else--> release, after the holding time is satisfied, start holding and update the holding
 */
UCLASS()
class TEAMASSIGNMENTFPS_API UTapHoldReleaseHelper : public UObject
{
	GENERATED_BODY()

public:

	/** Delegates for C++-only use */
	DECLARE_DELEGATE(FOnTap);
	DECLARE_DELEGATE(FOnHoldStart);
	DECLARE_DELEGATE_OneParam(FOnHoldUpdate, float /*HoldTime*/);
	DECLARE_DELEGATE(FOnRelease);

	FOnTap OnTap;
	FOnHoldStart OnHoldStart;
	FOnHoldUpdate OnHoldUpdate;
	FOnRelease OnRelease;

	FTimerHandle HoldTickHandle;

	// === Configuration ===
	// Minimum time in seconds to consider the press as a Hold instead of a Tap
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|TapHold")
	float TapThreshold = 0.3f;

	// How often to call OnHoldUpdate (seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|TapHold")
	float HoldTickInterval = 0.05f;

	// === Runtime State ===
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Input|TapHold")
	bool bPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Input|TapHold")
	bool bHoldStarted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Input|TapHold")
	float HoldTime = 0.f;
public:
	void ProcessInput(UWorld* World, bool bIsPressed);

private:
	void StartHoldTick(UWorld* World);
	void StopHoldTick(UWorld* World);
	void TickHoldUpdate();

	void BeginDestroy();// cleanup


};