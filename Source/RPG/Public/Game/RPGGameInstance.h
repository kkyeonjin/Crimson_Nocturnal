// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadGameRequested);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveGameRequested);

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

    // 사운드 관리
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSubclassOf<UBGMManager> BGMManagerClass;  // ★ TSubclassOf!

    UPROPERTY(BlueprintReadWrite, Category = "Audio")
    UBGMManager* BGMManager;  // 런타임 인스턴스

    //Func to Communicate with BGMManager
    void PlayHospitalBGM();
    void PlayNecropolisBGM();
    void PlayCathedralBGM();
    void PlayBossBGM();
    void StopBGM();

    UPROPERTY(BlueprintReadWrite)
    bool bHasSeenTutorial = false;

    UFUNCTION(BlueprintCallable)
    bool IsFirstPlay() { return !bHasSeenTutorial; }

    UFUNCTION(BlueprintCallable)
    void SetTutorialSeen(bool HasSeenTuto) { bHasSeenTutorial = HasSeenTuto; }

    // SaveGame 데이터 (포인터로 관리)
    UPROPERTY()
    USaveGameData* SaveGameData;

    // 세이브/로드 함수
    void RequestSaveGame();
    void RequestLoadGame();
    void ClearSaveData();
    bool HasSaveData();

    // 슬롯 이름
    UPROPERTY()
    FString SaveSlotName = TEXT("SaveSlot_01");
};
