#include "Game/RPGGameInstance.h"
#include "Game/BGMManager.h"
#include "Game/SaveGameData.h"
#include "Kismet/GameplayStatics.h"


void URPGGameInstance::Init()
{
	Super::Init();

	if (BGMManagerClass && !BGMManager)
	{
		BGMManager = NewObject<UBGMManager>(this, BGMManagerClass);
		UE_LOG(LogTemp, Warning, TEXT("BGMManager Created: %s"),
			BGMManager ? TEXT("OK") : TEXT("FAIL"));
	}

	if (SaveGameDataClass && !SaveGameData)
	{
		SaveGameData = NewObject<USaveGameData>(this, SaveGameDataClass);

		bHasSeenTutorial = SaveGameData->IsTutorialChecked;
	}
}

void URPGGameInstance::Shutdown()
{
	RequestSaveGame();
	Super::Shutdown();
}

void URPGGameInstance::RequestSaveGame()
{
	OnSaveGameRequested.Broadcast();
}

void URPGGameInstance::RequestLoadGame()
{
	OnLoadGameRequested.Broadcast();
}

void URPGGameInstance::ClearSaveData()
{
	SaveGameData->ClearData();
}

bool URPGGameInstance::HasSaveData()
{
	return UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0);
}

void URPGGameInstance::BackupPlayerData()
{
	// Caching Data for teleport to another level in runtime
	RunTimeSaveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));
}

//Send to AttributeComp
void URPGGameInstance::RestorePlayerData()
{
	OnRuntimeRestoreRequested.Broadcast(RunTimeSaveGameData);
}

bool URPGGameInstance::HasRunTimeSaveData()
{
	return RunTimeSaveGameData != nullptr;
}

void URPGGameInstance::PlayHospitalBGM()
{
	if (BGMManager) BGMManager->PlayBGM(BGMManager->HospitalBGM);
}

void URPGGameInstance::PlayNecropolisBGM()
{
	if (BGMManager) BGMManager->PlayBGM(BGMManager->NecropolisBGM);
}

void URPGGameInstance::PlayCathedralBGM()
{
	if (BGMManager) BGMManager->PlayBGM(BGMManager->CathedralBGM);
}

void URPGGameInstance::PlayBossBGM()
{
	if (BGMManager) BGMManager->PlayBGM(BGMManager->BossBGM);
}

void URPGGameInstance::PlaySaloonBGM()
{
	if (BGMManager) BGMManager->PlayBGM(BGMManager->SaloonBGM);
}

void URPGGameInstance::StopBGM()
{
	if (BGMManager) BGMManager->StopBGM();
}

void URPGGameInstance::SetTutorialSeen(bool HasSeenTuto)
{
	SaveGameData->SetTutorialSeen(HasSeenTuto);
}




