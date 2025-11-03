// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyState/EnemyTypes.h"
#include "Debug/UELOGCategories.h"
#include "Pooling/PoolingInterface.h"
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
class TEAMASSIGNMENTFPS_API AEnemyBaseCharacter : public ACharacter , public IPoolingInterface
{
	GENERATED_BODY()

public:
	AEnemyBaseCharacter();

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnemyState;

	FOnEnemyDead OnEnemyDead;
	FOnEnemyStateChanged OnEnemyStateChanged;

	FVector knockbackDirection;

	void InitializeEnemyData(FEnemyDataRow& InData); // Enemy ���� �� �� Enemy�� ������ �� ���� �޾� �ʱ�ȭ
	virtual void EnemyAttack();

	UFUNCTION()
	virtual void EnemyDead();

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
	virtual void EnemyTakeDamage(FDamageInfo DamageInfo);

	//void Knockback();
	
	//void EnemyDestroy();

private:

	void ChangeEnemyState(EEnemyState NewEnemyState);
	void DisableEnemyCollision();
public:

	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;
	
	FORCEINLINE FEnemyData GetEnemyData() const { return EnemyData; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
};


// enemy ĳ������ hit reaction�� ��� ���� ���⿡�� �´� �ִϸ��̼��� blend space�� ���� �� �� hit direaction�� ���� �´� �ִϸ��̼� ����