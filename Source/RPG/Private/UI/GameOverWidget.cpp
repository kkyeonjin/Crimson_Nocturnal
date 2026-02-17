#include "UI/GameOverWidget.h"
#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();

    //Change InputMode to UI InputMode
    this->SetKeyboardFocus();
    if (APlayerController* PC = GetOwningPlayer())
    {
        GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(this->TakeWidget());
        PC->SetInputMode(InputMode);
    }
}

void UGameOverWidget::ReStart()
{
    if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
    {
        if (GI->HasSaveData())
        {
            GI->RequestLoadGame();  
        }
        else
        {
            // Start new game if there's no SaveData
            GI->SetTutorialSeen(true);
            UGameplayStatics::OpenLevel(GetWorld(), FName("Hospital"));
        }
    }
}
