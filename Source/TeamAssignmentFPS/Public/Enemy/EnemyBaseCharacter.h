// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy/EnemyState/EnemyTypes.h"
#include "Debug/UELOGCategories.h"
#include "Interface/DamageInfo.h"
#include "Pooling/PoolingInterface.h"
//#include "Interface/InterfaceHP.h"
#include "EnemyBaseCharacter.generated.h"

DECLARE_DELEGATE_OneParam(FOnEnemyDead, int Score);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyStateChanged, EEnemyState NewState);

struct FEnemyDataRow;
struct FDamageInfo;

USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyType EnemyType;
	
	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float MoveSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float HeightMinRatio;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float HeightMaxRatio;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	int32 Damage;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float Range;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	float Delay;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
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

	bool bCanAttack = true;
	UFUNCTION()
	virtual void EnemyDead(FDamageInfo DamageInfo);
	UFUNCTION()
	virtual void EnemyDeadByPhaseEnd();// for death with no damage info
	
	void InitializeEnemyData(FEnemyDataRow* InData); // Enemy ���� �� �� Enemy�� ������ �� ���� �޾� �ʱ�ȭ
	void ChangeEnemyState(EEnemyState NewEnemyState);

	virtual void EndEnemySpawn();
	void EndChase();
	virtual void EnemyAttack();
	virtual void EnemyTakeDamage(FDamageInfo DamageInfo);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> d54ae87 (no message)
	virtual void EndHitReact();
	virtual void EnemyDead(FDamageInfo DamageInfo);
	void EndEnemyDead();
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
>>>>>>> 4c49a91 (11/11)

	// target actor
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Enemy | Target")
	AActor* TargetActor=nullptr;
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> b8ccb7b (feat enemy anim)
=======

>>>>>>> 83cc9c5 (delete)
=======
	virtual void EndHitReact();
	virtual void EnemyDead(FDamageInfo DamageInfo);
	void EndEnemyDead();
>>>>>>> 652a79a (Revert "delete")
=======

	void SetTargetActor(AActor* NewTargetActor);
	AActor* GetTargetActor()const{return TargetActor;}
>>>>>>> f79331c (weapon updated)
=======
>>>>>>> da14490 (Revert "Merge pull request from New_New-DevBranch")
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
=======
>>>>>>> 078c989 (11/10)
<<<<<<< HEAD
>>>>>>> 4c49a91 (11/11)
=======
=======

>>>>>>> 8c88e08 (11/10)
>>>>>>> d54ae87 (no message)
	

protected:
	virtual void BeginPlay() override;

	/*UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnemyState;*/

	AActor* GetTargetActor()const{return TargetActor;}
	void SetTargetActor(AActor* NewTargetActor) {TargetActor=NewTargetActor;}

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyType EnemyType;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	FEnemyData EnemyData;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHealthComponent> HealthComponent;

	virtual void EnemyAttackEnd();
	void LookAtPlayer();
	//void Knockback();
	
	//void EnemyDestroy();

private:

	
	void DisableEnemyCollision();
public:
	
	
	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;
	
	FORCEINLINE FEnemyData GetEnemyData() const { return EnemyData; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
};


// enemy ĳ������ hit reaction�� ��� ���� ���⿡�� �´� �ִϸ��̼��� blend space�� ���� �� �� hit direaction�� ���� �´� �ִϸ��̼� ����