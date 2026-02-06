// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/RPGGameInstance.h"
#include "Game/BGMManager.h"
#include "Game/SaveGameData.h"
#include "Kismet/GameplayStatics.h"


void URPGGameInstance::Init()
{
	Super::Init();

	// BGMManager 자동 생성
	if (BGMManagerClass && !BGMManager)
	{
		BGMManager = NewObject<UBGMManager>(this, BGMManagerClass);
		UE_LOG(LogTemp, Warning, TEXT("BGMManager Created: %s"),
			BGMManager ? TEXT("OK") : TEXT("FAIL"));
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

void URPGGameInstance::PlayHospitalBGM()
{
	if (BGMManager)
	{
		BGMManager->PlayBGM(BGMManager->HospitalBGM);
	}
}

void URPGGameInstance::PlayNecropolisBGM()
{
	if (BGMManager)
	{
		BGMManager->PlayBGM(BGMManager->NecropolisBGM);
	}
}

void URPGGameInstance::PlayCathedralBGM()
{
	if (BGMManager)
	{
		BGMManager->PlayBGM(BGMManager->CathedralBGM);
	}
}

void URPGGameInstance::PlayBossBGM()
{
	if (BGMManager)
	{
		BGMManager->PlayBGM(BGMManager->BossBGM);
	}
}


void URPGGameInstance::StopBGM()
{
	if (BGMManager)
	{
		BGMManager->StopBGM();
	}
}




