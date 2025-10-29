// Fill out your copyright notice in the Description page of Project Settings.
//sangwon


#include "Trap/TrapBase.h"

#include "Character/MyCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "CharacterStat/HealthComponent.h"
#include "Engine/OverlapResult.h"
#include "Interface/DamageInfo.h"

/**
 * 생성자
 * 트랩의 기본 컴포넌트를 초기화하고 NavMesh 영향을 비활성화
 */
ATrapBase::ATrapBase()
{
	// 매 프레임 Tick 비활성화 (성능 최적화)
	PrimaryActorTick.bCanEverTick = false;

	// 박스 콜리전 컴포넌트 생성 (캐릭터 오버랩 감지용)
	TrapCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("TrapCollision"));
	RootComponent = TrapCollision;
	TrapCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	TrapCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TrapCollision->SetCanEverAffectNavigation(false); // AI가 트랩을 피하지 않도록 NavMesh 영향 제거

	// 메시 컴포넌트 생성 (트랩 비주얼)
	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));
	TrapMesh->SetupAttachment(RootComponent);
	TrapMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 메시는 콜리전 비활성화
	TrapMesh->SetCanEverAffectNavigation(false); // AI가 트랩을 피하지 않도록 NavMesh 영향 제거
}

/**
 * 게임 시작 시 호출
 * 오버랩 이벤트 바인딩
 */
void ATrapBase::BeginPlay()
{
	Super::BeginPlay();

	// TrapCollision의 오버랩 이벤트를 OnTrapOverlap 함수에 바인딩
	if (TrapCollision)
	{
		TrapCollision->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnTrapOverlap);
	}
}

/**
 * 매 프레임 호출 (현재 비활성화됨)
 */
void ATrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrapBase::OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Check if the overlapping actor is a character
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	if (Character)
	{
		// Apply damage based on trap type
		if (DamageType == ETrapDamageType::SingleTarget)
		{
			ApplySingleTargetDamage(Character);
		}
		else if (DamageType == ETrapDamageType::AreaOfEffect)
		{
			ApplyAreaOfEffectDamage();
		}

		// Play explosion effect
		if (ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		}

		// Play explosion sound
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		}

		// Destroy the trap
		Destroy();
	}
}

void ATrapBase::ApplySingleTargetDamage(AMyCharacter* Character)
{
	if (!Character) return;

	// Get HealthComponent from the character
	UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>();
	if (HealthComp)
	{
		// Create damage info
		FDamageInfo DamageInfo;
		DamageInfo.DamageCauser = this;
		DamageInfo.DamageAmount = Damage;
		DamageInfo.DamageDirection = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();

		// Apply damage through HealthComponent
		HealthComp->GetDamage_Implementation(DamageInfo);
	}
}

void ATrapBase::ApplyAreaOfEffectDamage()
{
	if (!GetWorld()) return;

	// Perform sphere overlap to find all characters in range
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AOERadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape,
		QueryParams
	);

	// Track characters that have already been damaged
	TSet<class AMyCharacter*> DamagedCharacters;

	// Apply damage to all overlapped characters
	for (const FOverlapResult& Result : OverlapResults)
	{
		AMyCharacter* Character = Cast<AMyCharacter>(Result.GetActor());
		if (Character)
		{
			// Skip if already damaged
			if (DamagedCharacters.Contains(Character))
				continue;

			UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				// Create damage info
				FDamageInfo DamageInfo;
				DamageInfo.DamageCauser = this;
				DamageInfo.DamageAmount = Damage;
				DamageInfo.DamageDirection = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();

				// Apply damage through HealthComponent
				HealthComp->GetDamage_Implementation(DamageInfo);

				// Add to damaged characters set
				DamagedCharacters.Add(Character);
			}
		}
	}
}

