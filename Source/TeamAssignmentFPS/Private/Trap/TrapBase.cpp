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
#include "Enemy/EnemyBaseCharacter.h"
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

/**
 * 오버랩 이벤트 핸들러
 * 캐릭터가 트랩과 오버랩하면 트랩 발동
 */
void ATrapBase::OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	// 오버랩한 액터가 캐릭터인지 확인
	AMyCharacter* Character = Cast<AMyCharacter>(OtherActor);
	AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(OtherActor);

	if (Character || Enemy)
	{
		// 트랩 타입에 따라 데미지 적용
		if (DamageType == ETrapDamageType::SingleTarget)
		{
			ApplySingleTargetDamage(Character); // 단일 타겟 데미지
		}
		else if (DamageType == ETrapDamageType::AreaOfEffect)
		{
			ApplyAreaOfEffectDamage(); // 광역 데미지
		}

		// 폭발 이펙트 재생
		if (ExplosionEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
		}

		// 폭발 사운드 재생
		if (ExplosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
		}

		// 트랩 제거
		Destroy();
	}
}

/**
 * 단일 타겟 데미지 적용
 * 트랩을 밟은 플레이어나 Enemy 한 명에게 데미지를 줌
 */

void ATrapBase::ApplySingleTargetDamage(AActor* Target)
{
	if (!Target) return;

	//플레이어
	if (AMyCharacter* Character = Cast<AMyCharacter>(Target))
	{
		if (UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>())
		{
			FDamageInfo DamageInfo;
			DamageInfo.DamageCauser = this;
			DamageInfo.DamageAmount = Damage;
			DamageInfo.DamageDirection = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			HealthComp->GetDamage_Implementation(DamageInfo);
		}
	}

	// Enemy
	else if (AEnemyBaseCharacter* Enemy = Cast<AEnemyBaseCharacter>(Target))
	{
		FDamageInfo DamageInfo;
		DamageInfo.DamageCauser = this;
		DamageInfo.DamageAmount = Damage;
		DamageInfo.DamageDirection = (Enemy->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		
		Enemy->EnemyTakeDamage(DamageInfo);
	}
}
	

/**
 * 광역 데미지 적용
 * AOERadius 반경 내 모든 캐릭터에게 데미지를 줌
 * 같은 캐릭터에게 중복 데미지를 주지 않음
 */
void ATrapBase::ApplyAreaOfEffectDamage()
{
	if (!GetWorld()) return;

	// 구체 오버랩으로 반경 내 모든 캐릭터 찾기
	TArray<FOverlapResult> OverlapResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AOERadius);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this); // 자기 자신은 제외

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn); // Pawn 타입만 검색

	GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		CollisionShape,
		QueryParams
	);

	// 이미 데미지를 받은 캐릭터 추적 (중복 데미지 방지)
	TSet<class AMyCharacter*> DamagedCharacters;

	// 오버랩된 모든 캐릭터에게 데미지 적용
	for (const FOverlapResult& Result : OverlapResults)
	{
		AMyCharacter* Character = Cast<AMyCharacter>(Result.GetActor());
		if (Character)
		{
			// 이미 데미지를 받았다면 건너뛰기
			if (DamagedCharacters.Contains(Character))
				continue;

			UHealthComponent* HealthComp = Character->FindComponentByClass<UHealthComponent>();
			if (HealthComp)
			{
				// 데미지 정보 생성
				FDamageInfo DamageInfo;
				DamageInfo.DamageCauser = this; // 데미지 원인 (이 트랩)
				DamageInfo.DamageAmount = Damage; // 데미지 양
				DamageInfo.DamageDirection = (Character->GetActorLocation() - GetActorLocation()).GetSafeNormal(); // 데미지 방향

				// HealthComponent를 통해 데미지 전달
				HealthComp->GetDamage_Implementation(DamageInfo);

				// 데미지를 받은 캐릭터 목록에 추가
				DamagedCharacters.Add(Character);
			}
		}
	}
}

