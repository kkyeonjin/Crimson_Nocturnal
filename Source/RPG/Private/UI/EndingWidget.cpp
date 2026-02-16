// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EndingWidget.h"

#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UEndingWidget::NativeConstruct()
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

void UEndingWidget::EndGame()
{
    if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
    {
        GI->ClearSaveData();
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
}
