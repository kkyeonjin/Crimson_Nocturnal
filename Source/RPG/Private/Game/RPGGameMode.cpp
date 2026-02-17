#include "Game/RPGGameMode.h"
#include "Character/Player/PlayerCharacter.h"
#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/TutorialWidget.h"

//Called When New Map(Level) is open
void ARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

    if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
    {
        GI->RequestLoadGame();

        // Play BGM for each level
        FString LevelName = GetWorld()->GetMapName();

        if (LevelName.Contains(TEXT("Hospital")))
        {
            GI->PlayHospitalBGM();
            if (GI->IsFirstPlay())
            {
                ShowStandaloneTutorial();
            }
        }
        else if (LevelName.Contains(TEXT("Necropolis")))
        {
            GI->PlayNecropolisBGM();
        }
        else if (LevelName.Contains(TEXT("Cathedral")))
        {
            GI->PlayCathedralBGM();
        }
        else if (LevelName.Contains(TEXT("Saloon")))
        {
            GI->PlaySaloonBGM();
        }
    }
}

void ARPGGameMode::OnTutorialClosed()
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()))
    {
        Player->ShowStartPopUp();  
    }
}

void ARPGGameMode::BroadcastDialogue(const FString& DialogueName, const FString& DialogueLine)
{
    OnDialogueEvent.Broadcast(DialogueName, DialogueLine);
}

void ARPGGameMode::ShowStandaloneTutorial()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    // 1. Create TutorialWidget 
    if (TutorialUIClass)
    {
        TutorialWidget = CreateWidget<UTutorialWidget>(PC, TutorialUIClass);
        TutorialWidget->AddToViewport(200);  // Menu À§!

        TutorialWidget->OnTutorialClose.AddDynamic(this, &ARPGGameMode::OnTutorialClosed);

        // 2. Set InpuMode to UI Only
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TutorialWidget->TakeWidget());
        PC->SetInputMode(InputMode);

        // 3. Stop game play
        GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);
    }
}
