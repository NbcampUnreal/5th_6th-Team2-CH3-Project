// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBaseCharacter.h"
#include "RangeEnemyCharacter.generated.h"

class AProjectileBase;

UCLASS()
class TEAMASSIGNMENTFPS_API ARangeEnemyCharacter : public AEnemyBaseCharacter
{
	GENERATED_BODY()

public:
	ARangeEnemyCharacter();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USceneComponent> ProjectileSpawn;
	
protected:
	virtual void EnemyAttack() override;
	
private:
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USceneComponent> ProjectileSpawn;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AProjectileBase> Projectile;

	
	
	void LookAtPlayer();
};
