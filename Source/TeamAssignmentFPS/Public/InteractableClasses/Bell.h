// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystem.h"
#include "Components/SphereComponent.h"
#include "Bell.generated.h"

UCLASS()
class TEAMASSIGNMENTFPS_API ABell : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    ABell();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 인터페이스 구현
    virtual void Interact_Implementation(AActor* Interactor) override;

protected:
    // 구성요소
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BellMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* InteractionSphere;

    // 효과 관련
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    USoundBase* RingSound;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
    UParticleSystem* RingEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
    float InteractionRadius = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
    FName NextLevelName = "NextLevel";

    bool bHasRung = false;

    // 오버랩 이벤트
    UFUNCTION()
    void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                     bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};