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
class UAnimMontage;

UCLASS()
class TEAMASSIGNMENTFPS_API AEnemyBaseCharacter : public ACharacter , public IPoolingInterface
{
	GENERATED_BODY()

public:
	AEnemyBaseCharacter();

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	EEnemyState EnemyState;
	
	UFUNCTION()
	void EnemyDestroy();
	
	FOnEnemyDead OnEnemyDead;
	FOnEnemyStateChanged OnEnemyStateChanged;

	FVector knockbackDirection;
	
	void InitializeEnemyData(FEnemyDataRow* InData); // Enemy ���� �� �� Enemy�� ������ �� ���� �޾� �ʱ�ȭ
	void ChangeEnemyState(EEnemyState NewEnemyState);

	virtual void EndEnemySpawn();
	void EndChase();
	virtual void EnemyAttack();
	virtual void EndEnemyAttack();
	virtual void EnemyTakeDamage(FDamageInfo DamageInfo);
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> d54ae87 (no message)
=======
>>>>>>> 2bf2f5b (no message)
	virtual void EndHitReact();
	virtual void EnemyDead(FDamageInfo DamageInfo);
	void EndEnemyDead();
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 9dec185 (parabola weapon update)
=======
>>>>>>> 4c49a91 (11/11)
=======
>>>>>>> b24b0c4 (no message)
=======
>>>>>>> 6a4e438 (no message)
=======
=======
>>>>>>> 5d17d88 (11/10)
>>>>>>> 6018cf4 (no message)

	// target actor
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Enemy | Target")
	AActor* TargetActor=nullptr;
<<<<<<< HEAD
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
>>>>>>> 6018cf4 (no message)
=======
>>>>>>> 078c989 (11/10)
<<<<<<< HEAD
>>>>>>> 4c49a91 (11/11)
=======
=======

>>>>>>> 8c88e08 (11/10)
<<<<<<< HEAD
>>>>>>> d54ae87 (no message)
=======
=======
	virtual void EndHitReact();
	virtual void EnemyDead(FDamageInfo DamageInfo);
	void EndEnemyDead();
>>>>>>> 4237dcd (11/10)
<<<<<<< HEAD
>>>>>>> 2bf2f5b (no message)
=======
=======

	void SetTargetActor(AActor* NewTargetActor);
	AActor* GetTargetActor()const{return TargetActor;}
>>>>>>> b64879a (weapon updated)
<<<<<<< HEAD
>>>>>>> b24b0c4 (no message)
=======
=======
>>>>>>> f59b66a (11/10)
<<<<<<< HEAD
>>>>>>> 6a4e438 (no message)
=======
=======
>>>>>>> 5d17d88 (11/10)
>>>>>>> 6018cf4 (no message)
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds) override;
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
	
	FVector HitDirection;
	
	
	void PlayHitMontage(UAnimMontage* Montage);
	void PlayMontage(UAnimMontage* Montage);
	void StopMontage(UAnimMontage* Montage);
	void PlaySound(USoundBase* Sound);
	
	FRotator LookAtPlayer();
	void ReturnToChase();
	double GetKnockbackDireation(FVector Direction);

	

private:
	//void DisableEnemyCollision();

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	TObjectPtr<UAnimMontage> DeadMontage;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TObjectPtr<USoundBase> AttackSound;
public:
	
	
	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;
	
	FORCEINLINE FEnemyData GetEnemyData() const { return EnemyData; }
	FORCEINLINE EEnemyState GetEnemyState() const { return EnemyState; }
};


// enemy ĳ������ hit reaction�� ��� ���� ���⿡�� �´� �ִϸ��̼��� blend space�� ���� �� �� hit direaction�� ���� �´� �ִϸ��̼� ����