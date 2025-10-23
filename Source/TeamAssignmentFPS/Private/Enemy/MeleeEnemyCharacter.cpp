// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeEnemyCharacter.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	EnemyType = EEnemyType::EET_Melee;
}

void AMeleeEnemyCharacter::LoadEnemyDataRow()
{
	Super::LoadEnemyDataRow();
}
