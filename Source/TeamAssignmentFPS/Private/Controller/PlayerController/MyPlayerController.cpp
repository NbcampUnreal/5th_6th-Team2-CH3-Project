// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/PlayerController/MyPlayerController.h"
#include "Gamestate/GameStateManager.h"
#include "GameInstance/GameInstanceManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

//=== Managers ===//
#include "Camera/CameraManager.h"
#include "Camera/CameraRig.h"
#include "Controller/IMC/IMCManagerComponent.h"
#include "UI/UIManagerComponent.h"

//=== Debug ===//
#include "Debug/UELOGCategories.h"

//Time Helper
#include "Time/TimeControlHelper.h"



 AMyPlayerController::AMyPlayerController():
	//---- Manager Components ----//
	CameraManager(nullptr),
	IMCManager(nullptr),
	UIManager(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr), 
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr),
    NextMenuWidgetClass(nullptr),
    NextMenuWidgetInstance(nullptr),

	//----Control----//
	bIsGamepad(false),
	bIsInMenu(true)// start from the game menu so true, it is menu
 {
 	CameraManager=CreateDefaultSubobject<UCameraManagerComponent>(TEXT("Camera Manager Component"));
 	IMCManager=CreateDefaultSubobject<UIMCManagerComp>(TEXT("IMC Manager Component"));
 	UIManager=CreateDefaultSubobject<UUIManagerComp>(TEXT("UI Manager Component"));
 }

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

 	//==== Manager Comp Activate ===//

 	//Camera
 	ActivateCameraManager();
 
    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MenuLevel"))
    {
        ShowNextMenu(false);
        ShowMainMenu(false);
    }

<<<<<<< HEAD
<<<<<<< HEAD
   /* if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }

    AGameStateManager* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<AGameStateManager>() : nullptr;
    if (SpartaGameState)
    {
        SpartaGameState->UPdateHUD();
    }*/
=======
>>>>>>> 8621725 (widgtet updated)
=======
>>>>>>> e278224 (no message)
 	//== Activate UI Manager
 	UIManager->ActivateUIManager(this);
}

void AMyPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

 	//Deactivate ManagerComponents

 	UIManager->DeactivateUIManager();
=======
   /* if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();
        }
    }

    AGameStateManager* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<AGameStateManager>() : nullptr;
    if (SpartaGameState)
    {
        SpartaGameState->UPdateHUD();
    }*/
>>>>>>> 6cf3b6a (11/07 feat GameStateManager)
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

 	if (!InputComponent)//member varaible of this parent class(actor)
 	{
 		UE_LOG(IMC_Log,Error,TEXT("AMyPlayerController::SetupInputComponent->Invalid IC in SetupInputComponent for Checking GP or PC"));
 		return;
 	}
 	//this is only for checking which button is pressed. not to pawn function

 	//Key/button input detection
 	InputComponent->BindKey(EKeys::AnyKey, IE_Pressed,this, &AMyPlayerController::OnAnyInputPressed);

 	//mouse axis movement detection
 	InputComponent->BindAxisKey(EKeys::MouseX, this, &AMyPlayerController::OnMouseAxisMoved);
 	InputComponent->BindAxisKey(EKeys::MouseY, this, &AMyPlayerController::OnMouseAxisMoved);

 	//gamepad thumbstick axis movement detection
 	InputComponent->BindAxisKey(EKeys::Gamepad_LeftX, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 	InputComponent->BindAxisKey(EKeys::Gamepad_LeftY, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 	InputComponent->BindAxisKey(EKeys::Gamepad_RightX, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 	InputComponent->BindAxisKey(EKeys::Gamepad_RightY, this, &AMyPlayerController::OnGamePadControllerAxisMoved);
 		
 	UE_LOG(IMC_Log,Log,TEXT("AMyPlayerController::SetupInputComponent->Binding OnAnyInputPressed Completed"));
}

void AMyPlayerController::OnAnyInputPressed(FKey Key)
 {
 	if (Key.IsGamepadKey())//check if the updated key is from pc or gamepad
 	{
 		SetIsGamePad(true);
 	}
 	else// if (Key.IsMouseButton()||Key.IsKeyboardKey()) --> for now, default pc, gamepad
 	{
 		SetIsGamePad(false);
 	}
 }

void AMyPlayerController::OnMouseAxisMoved(float Value)
{
 	if (FMath::Abs(Value)<=KINDA_SMALL_NUMBER) return;// or use custom scalar value for threshold
 	
 	SetIsGamePad(false);
}

void AMyPlayerController::OnGamePadControllerAxisMoved(float Value)
{
 	if (FMath::Abs(Value)<=KINDA_SMALL_NUMBER) return;
 	
 	SetIsGamePad(true);
}


void AMyPlayerController::SetIsInMenu(bool bIsMenu)
{
 	
 	//TODO : Make a function to switch imc for menu control
 	//IMCManager-> change imc to menu or game
}

void AMyPlayerController::SetIsGamePad(bool NewbIsGamePad)
{
 	if (bIsGamepad==NewbIsGamePad)
 	{
 		//UE_LOG(IMC_Log,Error,TEXT("AMyPlayerController::SetIsGamePad->Already using %s"),*InputType);
 		return;
 	}
 	bIsGamepad=NewbIsGamePad;
 	
 	FString InputType=bIsGamepad? TEXT("Gamepad"):TEXT("PC/Keyboard");

 	UE_LOG(IMC_Log,Log,TEXT("Setted to %s"),*InputType);

 	if (UIManager)
 	{
 		UIManager->NotifyInputTypeChange(bIsGamepad);
 	}
 	/*if (IMCManager)
 	{
 		//IMCManager->Switch the imc for controll
 		//--> isnt gamepad and pc controll imc is already added and binded?--> no need for change when input type is changed
 	}*/
 	//OnInputTypeChanged.Broadcast(bIsGamepad);// broadcast to send signal to widgets to change the ui, and imc//--> already has the comp, give order directly for few confusion(for now)
}

void AMyPlayerController::ActivateCameraManager()
{
 	if (!CameraManager)
 	{
 		UE_LOG(Controller_Log, Error, TEXT("AMyPlayerController::ActivateCameraManager-> CameraManager is null. Activation Failed"));
 		return;
 	}

 	
 	CameraManager->ActivateCameraManager();

 	//Activate the camera of controlled pawn
 		
 	APawn* ControlledPawn = GetPawn();
 	if (!ControlledPawn)
 	{
 		UE_LOG(Camera_Log, Warning, TEXT("AMyPlayerController::ActivateCameraManager-> invalid Controlled pawn "));
 		return;
 	}

 	TArray<UChildActorComponent*> ChildActorComponents;// storage of what character has for childactorcomponents
 	ControlledPawn->GetComponents<UChildActorComponent>(ChildActorComponents);

 	for (UChildActorComponent* ChildActorComp : ChildActorComponents)//loop till it find the camera rig actor
 	{
 		if (!ChildActorComp) continue;
 		
 		ACameraRig* CameraRig = Cast<ACameraRig>(ChildActorComp->GetChildActor());
 		if (CameraRig)// if casting is successful, it is camera rig
 		{
 			CameraManager->SetActiveCameraRig(CameraRig);// set that as active camera rig
 			UE_LOG(Camera_Log, Log, TEXT("AMyPlayerController::ActivateCameraManager->Found CameraRig in ChildActorComponent, set as active."));
 			return;
 		}
 	}
 	//What if there is more that one camera rig? how can i set the starting camera?
 	//fuck

 	UE_LOG(Camera_Log, Warning, TEXT("AMyPlayerController::ActivateCameraManager->No CameraRig found on spawned pawn."));

}

UUserWidget* AMyPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

void AMyPlayerController::ShowGameHUD()
{
    // HUDï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ö´Ù¸ï¿½ ï¿½Ý±ï¿½
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    // ï¿½Ì¹ï¿½ ï¿½Þ´ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    // ÀÌ¹Ì ¸Þ´º°¡ ¶° ÀÖÀ¸¸é Á¦°Å
    if (NextMenuWidgetInstance)
    {
        NextMenuWidgetInstance->RemoveFromParent();
        NextMenuWidgetInstance = nullptr;
    }

    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());

            AGameStateManager* GameStateManager = GetWorld() ? GetWorld()->GetGameState<AGameStateManager>() : nullptr;
            if (GameStateManager)
            {
                GameStateManager->UPdateHUD();
            }
        }

    }
}

void AMyPlayerController::ShowMainMenu(bool bIsRestart)
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }

        if (UTextBlock* StartButteonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
        {
            if (bIsRestart)
            {
                StartButteonText->SetText(FText::FromString(TEXT("ReStart")));
            }
            else
            {
                StartButteonText->SetText(FText::FromString(TEXT("Start")));
            }
        }

        if (bIsRestart)
        {

            if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
            {
                if (UGameInstanceManager* GameInstance = Cast<UGameInstanceManager>(UGameplayStatics::GetGameInstance(this)))
                {
                    TotalScoreText->SetText(FText::FromString(
                        FString::Printf(TEXT("Total Score: %d"), GameInstance->TotalScore)
                    ));
                }
            }
        }
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->DisableInput(PC);   // ¡ç ÇÃ·¹ÀÌ¾î ÀÔ·Â ºñÈ°¼ºÈ­
        PC->bShowMouseCursor = true;
    }
}

void AMyPlayerController::StartGame()
{
    if (UGameInstanceManager* GameInstance = Cast<UGameInstanceManager>(UGameplayStatics::GetGameInstance(this)))
    {
        GameInstance->CurrentLevelIndex = 0;
        GameInstance->TotalScore = 0;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("EnemyTestMap"));
}

void AMyPlayerController::ShowNextMenu(bool bIsRestart)
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (NextMenuWidgetInstance)
    {
        NextMenuWidgetInstance->RemoveFromParent();
        NextMenuWidgetInstance = nullptr;
    }

    if (NextMenuWidgetClass)
    {
        NextMenuWidgetInstance = CreateWidget<UUserWidget>(this, NextMenuWidgetClass);
        if (NextMenuWidgetClass)
        {
            NextMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }
    }

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC)
    {
        PC->DisableInput(PC);   // ¡ç ÇÃ·¹ÀÌ¾î ÀÔ·Â ºñÈ°¼ºÈ­
        PC->bShowMouseCursor = true;
    }
}

