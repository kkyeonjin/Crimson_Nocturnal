#include "UI/GameOverWidget.h"
#include "Game/RPGGameInstance.h"
#include "Game/SaveGameData.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogTemp, Error, TEXT("GameOver NativeConstruct START"));

    SetIsFocusable(true);
    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(TakeWidget()); 
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
        SetKeyboardFocus();
    }
}

void UGameOverWidget::ReStart()
{
    UE_LOG(LogTemp, Display, TEXT("REStart!!!"));

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
