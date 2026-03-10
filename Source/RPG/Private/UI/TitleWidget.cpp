// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleWidget.h"
#include "Components/Image.h"
#include "Game/RPGGameInstance.h"
#include "Game/SaveGameData.h"
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

    MenuCircles[TitleMenuSelectCount]->SetVisibility(ESlateVisibility::Visible);

}

void UTitleWidget::OnSelect()
{
    if (TitleMenuSelectCount == 0) //start
    {
        if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
        {
            GI->ClearSaveData();  
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
        if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
        {
            if (GI->HasSaveData())
            {
                if (auto PlayerGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0)))
                {
                    FString RawMapName = PlayerGameData->CurrentMap;

                    FString CleanMapName = RawMapName;
                    CleanMapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
                    CleanMapName.RemoveFromStart(TEXT("PersistentLevel::"));

                    UE_LOG(LogTemp, Error, TEXT("CLEAN MAP NAME: '%s'"), *CleanMapName);

                    FName SavedMapName = FName(*CleanMapName);

                    UGameplayStatics::OpenLevel(GetWorld(), SavedMapName);
                }
            }
            else
            {
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

