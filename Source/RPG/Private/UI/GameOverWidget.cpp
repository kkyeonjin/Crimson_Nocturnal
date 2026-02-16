// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOverWidget.h"

#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
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
}

void UGameOverWidget::ReStart()
{
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
