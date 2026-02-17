// GI that exists all the time during runtime
// Manage load/save/BGM play

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadGameRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRuntimeRestoreRequested, USaveGameData*, RuntimeSaveGameData);

class UBGMManager;
class USaveGameData;

UCLASS()
class RPG_API URPGGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    FOnLoadGameRequested OnLoadGameRequested;
    FOnSaveGameRequested OnSaveGameRequested;
    FOnRuntimeRestoreRequested OnRuntimeRestoreRequested;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSubclassOf<UBGMManager> BGMManagerClass; 

    UPROPERTY(BlueprintReadWrite, Category = "Audio")
    UBGMManager* BGMManager;  

    //Func to Communicate with BGMManager
    void PlayHospitalBGM();
    void PlayNecropolisBGM();
    void PlayCathedralBGM();
    void PlayBossBGM();
    void PlaySaloonBGM();
    void StopBGM();

    UPROPERTY(BlueprintReadWrite)
    bool bHasSeenTutorial = false;

    UFUNCTION(BlueprintCallable)
    bool IsFirstPlay() { return !bHasSeenTutorial; }

    UFUNCTION(BlueprintCallable)
    void SetTutorialSeen(bool HasSeenTuto);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<USaveGameData> SaveGameDataClass; 

    UPROPERTY(BlueprintReadWrite)
    USaveGameData* SaveGameData;  

    //for runtime backup
    UPROPERTY()
    USaveGameData* RunTimeSaveGameData;

    // Load & Save
    void RequestSaveGame();
    void RequestLoadGame();
    void ClearSaveData();
    bool HasSaveData();

    //when new map loaded
    void BackupPlayerData();
    void RestorePlayerData();
    bool HasRunTimeSaveData();

    UPROPERTY()
    FString SaveSlotName = TEXT("SaveSlot_01");
};
