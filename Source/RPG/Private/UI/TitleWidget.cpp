// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "Components/Image.h"
#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //Change InputMode to UI InputMode
	//for Use of BaseUIInputWidget::NativeOnKeyDown 
    SetIsFocusable(true);
    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget());
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }

    MenuCircles = { StartCircle, ContinueCircle, ExitCircle };
}

void UTitleWidget::OnNavigate(int32 Input)
{
    MenuCircles[TitleMenuSelectCount]->SetVisibility(ESlateVisibility::Hidden);

    TitleMenuSelectCount -= Input;

    if (TitleMenuSelectCount == -1)
    {
        TitleMenuSelectCount = 2;
    }
    else if (TitleMenuSelectCount == MENUNUM)
    {
        TitleMenuSelectCount = 0;
    }

    //UE_LOG(LogTemp, Warning, TEXT("TitleMenuCount: %d"), TitleMenuSelectCount);

    MenuCircles[TitleMenuSelectCount]->SetVisibility(ESlateVisibility::Visible);

}

void UTitleWidget::OnSelect()
{
    if (TitleMenuSelectCount == 0) //start
    {
        if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
        {
            GI->ClearSaveData();  // 저장 삭제
            GI->SetTutorialSeen(false);
            UGameplayStatics::OpenLevel(GetWorld(), FName("Hospital"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("No GAME INSTANCE"));
        }
    }
    else if (TitleMenuSelectCount == 1) //continue
    {
        UE_LOG(LogTemp, Display, TEXT("Continue"));
        if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
        {
            if (GI->HasSaveData())
            {
                GI->RequestLoadGame();  // 저장된 데이터 로드
            }
            else
            {
                // 저장 없으면 새 게임
                GI->SetTutorialSeen(false);
                UGameplayStatics::OpenLevel(GetWorld(), FName("Hospital"));
            }
        }
    }
    else //exit
    {
        FGenericPlatformMisc::RequestExit(false);
    }
}

