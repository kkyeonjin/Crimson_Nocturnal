// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RPGGameMode.h"
#include "Game/RPGGameInstance.h"
#include "UI/TutorialWidget.h"

//Called When New Map is open
void ARPGGameMode::BeginPlay()
{
	Super::BeginPlay();

    if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
    {
        // 현재 맵에 맞는 BGM 재생
        FString LevelName = GetWorld()->GetMapName();

        if (LevelName.Contains(TEXT("Hospital")))
        {
            GI->PlayHospitalBGM();
            if (GI->IsFirstPlay())
            {
                // MenuWidget 없이 Tutorial만!
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
        
        
    }
}

void ARPGGameMode::ShowStandaloneTutorial()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC) return;

    // 1. TutorialWidget 독립 생성
    if (TutorialUIClass)
    {
        TutorialWidget = CreateWidget<UTutorialWidget>(PC, TutorialUIClass);
        TutorialWidget->AddToViewport(200);  // Menu 위!

        // 2. UI Only 모드
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TutorialWidget->TakeWidget());
        PC->SetInputMode(InputMode);

        // 3. 시간 정지
        GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);
    }
}
