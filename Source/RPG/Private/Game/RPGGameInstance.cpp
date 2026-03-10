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
	}

	if (HasSaveData())
	{
		SaveGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));
	}
	else
	{
		SaveGameData = NewObject<USaveGameData>(this, SaveGameDataClass);
	}

	bHasSeenTutorial = SaveGameData->IsTutorialChecked;
}

void URPGGameInstance::Shutdown()
{
	RequestSaveGame();
	FGenericPlatformMisc::RequestExit(false);
}

URPGGameInstance::~URPGGameInstance()
{
	UE_LOG(LogTemp, Warning, TEXT("GI ~DTOR: BGM=%p SAVE=%p"), BGMManager, SaveGameData);
}

void URPGGameInstance::RequestSaveGame()
{
	//broadcast to PlayerAttributeComp
	OnSaveGameRequested.Broadcast();
}

void URPGGameInstance::RequestLoadGame()
{
	//broadcast to PlayerAttributeComp
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
	bHasSeenTutorial = HasSeenTuto;
}




