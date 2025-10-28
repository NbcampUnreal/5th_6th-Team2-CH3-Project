// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyState/EnemyTypes.h"
#include "Debug/UELOGCategories.h"
//#include "Interface/InterfaceHP.h"
#include "EnemyBaseCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnEnemyDead, int Score);
DECLARE_DELEGATE_OneParam(FOnEnemyStateChanged, EEnemyState NewState);

struct FEnemyDataRow;
struct FDamageInfo;

USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Enemy")
	EEnemyType EnemyType;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 MaxHealth;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 CurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float HeightMinRatio;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float HeightMaxRatio;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 Damage;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float Range;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	float Delay;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 Score;
};


class UHealthComponent;

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemyBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBaseCharacter();
	//AEnemyBaseCharacter(FEnemyDataRow& InData);

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnemyState;

	FOnEnemyDead OnEnemyDead;
	FOnEnemyStateChanged OnEnemyStateChanged;

	FVector knockbackDirection;

	void InitializeEnemyData(FEnemyDataRow& InData); // Enemy 생성 할 때 Enemy의 데이터 값 전달 받아 초기화
	virtual void EnemyAttack();

	UFUNCTION()
	void EnemyDead();

protected:
	virtual void BeginPlay() override;

	/*UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnemyState;*/

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyType EnemyType;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	FEnemyData EnemyData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	virtual void EnemyAttackEnd();

	void EnemyTakeDamage(FDamageInfo DamageInfo);

private:

	void ChangeEnemyState(EEnemyState NewEnemyState);

public:

	FORCEINLINE FEnemyData GetEnemyData() const { return EnemyData; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
};


// enemy 캐릭터의 hit reaction의 경우 여러 방향에서 맞는 애니메이션을 blend space로 연결 한 후 hit direaction에 따라 맞는 애니메이션 실행