// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrapBase.generated.h"

/**
 * 트랩 데미지 타입 열거형
 * 트랩이 데미지를 적용하는 방식을 결정
 */
UENUM(BlueprintType)
enum class ETrapDamageType : uint8
{
	/** 트랩을 밟은 캐릭터 한 명만 데미지 */
	SingleTarget UMETA(DisplayName = "Single Target"),

	/** 반경 내 모든 캐릭터에게 광역 데미지 */
	AreaOfEffect UMETA(DisplayName = "Area of Effect")
};

/**
 * ATrapBase
 * 캐릭터와 오버랩 시 데미지를 주는 트랩 베이스 클래스
 *
 * 주요 기능:
 * - 단일 타겟/광역 데미지 두 가지 모드 지원
 * - NavMesh 영향 제거 (AI가 트랩을 피하지 않음)
 * - 폭발 이펙트 및 사운드 재생
 * - HealthComponent를 통한 데미지 전달
 *
 * 사용 방법:
 * 1. ATrapBase를 부모로 하는 블루프린트 생성
 * 2. TrapMesh에 원하는 메시 설정
 * 3. DamageType, Damage, AOERadius(광역인 경우) 설정
 * 4. ExplosionEffect와 ExplosionSound 할당
 * 5. 레벨에 배치
 */
UCLASS()
class TEAMASSIGNMENTFPS_API ATrapBase : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * 생성자
	 * 컴포넌트 초기화 및 NavMesh 영향 비활성화
	 */
	ATrapBase();

protected:
	/**
	 * 게임 시작 시 또는 스폰될 때 호출
	 * TrapCollision에 오버랩 이벤트 바인딩
	 */
	virtual void BeginPlay() override;

	// ========== 컴포넌트 ==========

	/*
	 * 캐릭터 오버랩 감지용 박스 콜리전 컴포넌트
	 * 기본 크기: 50x50x50
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* TrapCollision;

	/*
	 * 트랩의 비주얼 메시 컴포넌트
	 * 블루프린트에서 원하는 외형으로 설정
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* TrapMesh;

	// ========== 트랩 설정 ==========

	/*
	 * 데미지 적용 방식
	 * - SingleTarget: 트랩을 밟은 캐릭터만 데미지
	 * - AreaOfEffect: AOERadius 내 모든 캐릭터에게 데미지
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
	ETrapDamageType DamageType = ETrapDamageType::SingleTarget;

	/*
	 * 적용할 데미지 양
	 * 기본값: 50.0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
	float Damage = 50.0f;

	/*
	 * 광역 데미지 반경 (언리얼 유닛)
	 * DamageType이 AreaOfEffect일 때만 표시됨
	 * 기본값: 500.0
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings", meta = (EditCondition = "DamageType == ETrapDamageType::AreaOfEffect", EditConditionHides))
	float AOERadius = 500.0f;

	/**
	 * 트랩 발동 시 재생할 파티클 이펙트
	 * 선택사항 - 원하는 폭발/발동 이펙트 설정
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
	class UParticleSystem* ExplosionEffect;

	/**
	 * 트랩 발동 시 재생할 사운드
	 * 선택사항 - 원하는 폭발/발동 사운드 설정
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trap Settings")
	class USoundBase* ExplosionSound;

	// ========== 트랩 함수 ==========

	/**
	 * TrapCollision에 액터가 오버랩될 때 호출
	 * 오버랩한 액터가 AMyCharacter인 경우 트랩 발동
	 *
	 * @param OverlappedComponent - 오버랩된 콜리전 컴포넌트
	 * @param OtherActor - 오버랩한 액터
	 * @param OtherComp - 오버랩한 액터의 컴포넌트
	 * @param OtherBodyIndex - 바디 인덱스
	 * @param bFromSweep - 스윕으로부터 발생했는지 여부
	 * @param SweepResult - 스윕인 경우 히트 결과
	 */
	UFUNCTION()
	void OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	/**
	 * 단일 캐릭터에게 데미지 적용
	 * HealthComponent를 통해 FDamageInfo로 데미지 전달
	 *
	 * @param Character - 데미지를 받을 캐릭터
	 */
	void ApplySingleTargetDamage(class AMyCharacter* Character);

	/**
	 * AOERadius 내 모든 캐릭터에게 데미지 적용
	 * 구체 오버랩으로 범위 내 모든 Pawn 액터 탐색
	 * 같은 캐릭터에게 중복 데미지 방지
	 */
	void ApplyAreaOfEffectDamage();

public:
	/** 매 프레임 호출 (현재 성능을 위해 비활성화) */
	virtual void Tick(float DeltaTime) override;

};
