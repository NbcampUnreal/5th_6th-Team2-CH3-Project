#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
    GENERATED_BODY()
};

/**
 * 인터랙션 가능한 객체용 인터페이스
 */
class TEAMASSIGNMENTFPS_API IInteractable
{
    GENERATED_BODY() 

public:
    /** 
    플레이어와 상호작용할 때 호출됨
    */
    virtual void Interact(class AMyCharacter* Player) = 0;
};
