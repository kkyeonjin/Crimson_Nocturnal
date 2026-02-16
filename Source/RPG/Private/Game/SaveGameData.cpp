// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SaveGameData.h"

#include "Kismet/GameplayStatics.h"

USaveGameData::USaveGameData()
{

}

void USaveGameData::ClearData()
{
	//if (!this) return;

	SaveSlotName = TEXT("PlayerSaveSlot");

	UserIndex = 0;

	SavedCurrentLevel = 1;

	SavedCurrentExp = 0;

	ActivatedSkills = { 0, 0, 0, 0, 0 };

	SavedCurrentHP = 120;
		
	SavedCurrentBP = 0;

	SavedMaxHP = 120;

	SavedMaxBP = 0;

	SavedATK = 12;

	SavedDEF = 2;

	SavedSkillPoint = 0;

	EquippedUlt = 10;

	CurrentMap = TEXT("Hospital");

	Ult1XSetting = 0;

	Ult1YSetting = 0;

	UGameplayStatics::DeleteGameInSlot(TEXT("PlayerSaveSlot"), 0);
}

