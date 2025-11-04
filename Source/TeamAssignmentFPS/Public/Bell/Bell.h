// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MyCharacter.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"
#include "Bell.generated.h"




/**
 * ABell
 * 플레이어가 근처에서 E키를 눌러 상호작용할 수 있는 종(Bell)
 * 상호작용 시 다음 웨이브 / 다음 페이지로 진행됨
 */
UCLASS()
class TEAMASSIGNMENTFPS_API ABell : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    /** 
    생성자 
    */
    ABell();


protected:
    /** 
    게임 시작 시 호출 
    */
    virtual void BeginPlay() override;

    

    /** 
    종 메쉬 
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* BellMesh;

    /**
    상호작용 범위 감지를 위한 Sphere 콜리전 
    */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* InteractionSphere;


    /** 
    상호작용 가능 거리 
    (Sphere 반경) 
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bell Settings")
    float InteractionRadius = 200.0f;

    /** 
    상호작용 시 재생할 사운드
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bell Settings")
    class USoundBase* RingSound;

    /** 
    상호작용 시 재생할 파티클 
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bell Settings")
    class UParticleSystem* RingEffect;

    /** 
    이미 울렸는지 여부 
    (중복 방지) 
    */
    bool bHasRung = false;

public:
    /** 
    Tick ? 거리 감지나 실시간 상호작용용 
    */
    virtual void Tick(float DeltaTime) override;

    /** 
    인터페이스 함수 구현
    (E 키 입력 시) 
    */
  virtual void Interact_Implementation(AActor* Interactor);

    /** 
    Sphere 오버랩 
    플레이어 감지 
    */
    UFUNCTION()
    void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
    void Interact(AMyCharacter* Player);
};