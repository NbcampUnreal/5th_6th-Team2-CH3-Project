// Fill out your copyright notice in the Description page of Project Settings.

#include "Bell/Bell.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/MyCharacter.h"
#include "GameFramework/PlayerController.h"

ABell::ABell()
{
    PrimaryActorTick.bCanEverTick = false;

    BellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BellMesh"));
    RootComponent = BellMesh;

    InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
    InteractionSphere->SetupAttachment(BellMesh);
    InteractionSphere->InitSphereRadius(InteractionRadius);
    
    UE_LOG(LogTemp, Warning, TEXT("Bell Mesh initialized"));
    
    InteractionSphere->SetCollisionProfileName(TEXT("Trigger"));

    // 오버랩 이벤트 바인딩
    InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABell::OnSphereOverlapBegin);
    InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ABell::OnSphereOverlapEnd);
}

void ABell::BeginPlay()
{
    Super::BeginPlay();
}

void ABell::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABell::Interact_Implementation(AActor* Interactor)
{
	AMyCharacter* Player = Cast<AMyCharacter>(Interactor);
    if (Player);

	if (bHasRung) return; // 중복 방지
	bHasRung = true;

	UE_LOG(LogTemp, Warning, TEXT("플레이어와 상호작용  종 울리기"));


    // 효과 재생
    if (RingSound)
        UGameplayStatics::PlaySoundAtLocation(this, RingSound, GetActorLocation());

    if (RingEffect)
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), RingEffect, GetActorTransform());

	// "다음 페이지" 기능 수행
	UGameplayStatics::OpenLevel(this, FName("NextLevel")); // 예시: Next Level로 이동

	UE_LOG(LogTemp, Warning, TEXT("다음 페이지로 이동"));
}

void ABell::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
    const FHitResult& SweepResult)
{
    if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Log, TEXT("E키로 상호작용 가능"));
        // 여기서 PlayerHUD에 "E키를 눌러 종을 울리세요" 표시 가능
    }
}

void ABell::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (AMyCharacter* Player = Cast<AMyCharacter>(OtherActor))
    {
        UE_LOG(LogTemp, Log, TEXT("플레이어가 벨 범위를 벗어남"));
        // HUD 표시 제거 가능
    }
}

