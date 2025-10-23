
#include "Enemy/EnemyBaseCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "Debug/UELOGCategories.h"
#include "GameFramework/CharacterMovementComponent.h"


AEnemyBaseCharacter::AEnemyBaseCharacter()
{
	
	//PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	//Movement->bOrientRotationToMovement = true;

	Movement->MaxWalkSpeed = 300.f;

	EnemyState = EEnemyState::EES_Idle;

	//ConstructorHelpers::FObjectFinder<UDataTable>
	//	DataTable(TEXT("/Game/Enemy/DT_EnemyData.DT_EnemyData"));
	//if (DataTable.Succeeded())
	//{
	//	EnemyDataTable = DataTable.Object;
	//	UE_LOG(Enemy_Log, Warning, TEXT("Enemy DataTable Load Success"));
	//}
	//else
	//{
	//	UE_LOG(Enemy_Log, Warning, TEXT("Enemy DataTable Load Faile"));
	//}
}

void AEnemyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	LoadEnemyDataRow();

	UE_LOG(Enemy_Log, Warning, TEXT("%f"), GetEnemyAttackRange());

}

void AEnemyBaseCharacter::LoadEnemyDataRow()
{
	if (!EnemyDataTable)
	{
		return;
	}

	TArray<FEnemyDataRow*> AllRows;
	static const FString ContextString(TEXT("FEnemyDataContext"));
	EnemyDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return;

	for (FEnemyDataRow* Row : AllRows)
	{
		if (EnemyType == Row->EnemyType)
		{
			EnemyData = Row;
		}
	}
}


