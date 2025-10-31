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

void AMeleeEnemyCharacter::EnableAttackCollision() 
{
	if (!AttackCollision)
	{
		return;
	}

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AMeleeEnemyCharacter::DisableAttackCollision() 
{
	if (!AttackCollision)
	{
		return;
	}

	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AMeleeEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();


	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeEnemyCharacter::OnAttackOverlap);

}

void AMeleeEnemyCharacter::EnemyTakeDamage(FDamageInfo DamageInfo)
{
	Super::EnemyTakeDamage(DamageInfo);

	DisableAttackCollision();
	UE_LOG(Enemy_Log, Error, TEXT("Melee Damaged"));
}

void AMeleeEnemyCharacter::EnemyDead()
{
	Super::EnemyDead();

	DisableAttackCollision();

	UE_LOG(Enemy_Log, Error, TEXT("Melee Dead"));
}


void AMeleeEnemyCharacter::OnAttackOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (EnemyState != EEnemyState::EES_Attack)
	{
		UE_LOG(Enemy_Log, Warning, TEXT("not Attack State"));
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
			UE_LOG(Enemy_Log, Warning, TEXT("Attack Success"));

			Interface->Execute_GetDamage(HealthComp, DamageInfo);
		}
	}

}


// ���� Enemy�� �ִϸ��̼� �߰� �� motion warping�� ����ؼ� �÷��̾� ĳ������ �������� ������ �� �� �ֵ��� ����
// uprimitivecomponent collision component;