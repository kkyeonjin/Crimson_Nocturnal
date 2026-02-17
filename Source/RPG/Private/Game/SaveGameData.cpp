#include "Game/SaveGameData.h"
#include "Kismet/GameplayStatics.h"


void USaveGameData::ClearData()
{
	if (!this) return;
	UGameplayStatics::DeleteGameInSlot(TEXT("PlayerSaveSlot"), 0);
}

void USaveGameData::SetTutorialSeen(bool HasSeenTuto)
{
	IsTutorialChecked = HasSeenTuto;
}

