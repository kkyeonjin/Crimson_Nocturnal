#include "UI/EndingWidget.h"
#include "Game/RPGGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UEndingWidget::NativeConstruct()
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

void UEndingWidget::EndGame()
{
    if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
    {
        GI->ClearSaveData();
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("Title"));
}
