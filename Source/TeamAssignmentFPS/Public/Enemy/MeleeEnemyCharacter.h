// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "MeleeEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TEAMASSIGNMENTFPS_API AMeleeEnemyCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()
	
public:
	AMeleeEnemyCharacter();

	void EnableAttackCollision();
	void DisableAttackCollision();

protected:
	virtual void BeginPlay() override;

	virtual void EnemyAttack() override;
	virtual void EnemyDead(FDamageInfo DamageInfo) override;
	virtual void EnemyTakeDamage(FDamageInfo DamageInfo) override;
private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> AttackCollision;


	UFUNCTION()
	void OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
