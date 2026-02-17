// SaveData that saved in game file permanently
// Used in Load&Save / runtime load& save

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGameData.generated.h"

UCLASS()
class RPG_API USaveGameData : public USaveGame
{
	GENERATED_BODY()

public:
	void ClearData();

	void SetTutorialSeen(bool HasSeenTuto);

	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	uint32 UserIndex;

	UPROPERTY()
	int32 SavedCurrentLevel;

	UPROPERTY()
	int32 SavedCurrentExp;

	UPROPERTY()
	TArray<int32> ActivatedSkills = { 0, 0, 0, 0, 0 };

	UPROPERTY()
	int32 SavedCurrentHP;

	UPROPERTY()
	int32 SavedCurrentBP;

	UPROPERTY()
	int32 SavedMaxHP;

	UPROPERTY()
	int32 SavedMaxBP;

	UPROPERTY()
	int32 SavedATK;

	UPROPERTY()
	int32 SavedDEF;

	UPROPERTY()
	int32 SavedSkillPoint;

	UPROPERTY()
	int32 EquippedUlt;

	UPROPERTY()
	FString CurrentMap;

	UPROPERTY()
	int32 Ult1XSetting;

	UPROPERTY()
	int32 Ult1YSetting;

	UPROPERTY()
	bool IsTutorialChecked = false;

};
