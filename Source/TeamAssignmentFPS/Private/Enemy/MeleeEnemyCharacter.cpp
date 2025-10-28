// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MeleeEnemyCharacter.h"
#include "Components/CapsuleComponent.h"
#include "CharacterStat/HealthComponent.h"

AMeleeEnemyCharacter::AMeleeEnemyCharacter()
{
	EnemyType = EEnemyType::EET_Melee;

	AttackCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Attack Collision"));
	AttackCollision->SetupAttachment(RootComponent);
}

void AMeleeEnemyCharacter::AttackCollisionOn()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeEnemyCharacter::AttackCollisionOff()
{
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemyCharacter::OnAttackOverlap);

}

void AMeleeEnemyCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (EnemyState != EEnemyState::EES_Attack)
	{
		return;
	}


	if (!OtherActor)
	{
		UE_LOG(Enemy_Log, Warning, TEXT("OtherActor null"));

		return;
	}

	FDamageInfo DamageInfo;

	DamageInfo.DamageAmount = GetEnemyData().Damage;

	UHealthComponent* HealthComp = OtherActor->FindComponentByClass<UHealthComponent>();
	if (HealthComp && HealthComp->GetClass()->ImplementsInterface(UInterfaceHP::StaticClass()))
	{
		if (IInterfaceHP* Interface = Cast<IInterfaceHP>(HealthComp))
		{
			UE_LOG(Enemy_Log, Warning, TEXT("Attck Success"));

			Interface->Execute_GetDamage(HealthComp, DamageInfo);
		}
	}

}


