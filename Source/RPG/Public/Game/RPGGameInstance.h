// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoundManager.h"
#include "RPGGameInstance.generated.h"

UCLASS()
class RPG_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

    // 사운드 관리
    UPROPERTY()
    class USoundManager* SoundManager;

    // 현재 게임 데이터 (메모리에서 관리)
    UPROPERTY()
    int32 PlayerLevel = 1;

    UPROPERTY()
    int32 PlayerExp = 0;

    UPROPERTY()
    int32 PlayerCurrentHP = 100;

    UPROPERTY()
    int32 PlayerMaxHP = 100;

    UPROPERTY()
    FString CurrentMapName = TEXT("Map_01");

    UPROPERTY()
    FVector PlayerSavePosition = FVector::ZeroVector;

    UPROPERTY()
    bool bBossDefeated = false;

    // SaveGame 데이터 (포인터로 관리)
    UPROPERTY()
    class USaveGameData* SaveGameData;

    // 세이브/로드 함수
    void SaveGame();
    void LoadGame();
    void ClearGameData();

    // 슬롯 이름
    UPROPERTY()
    FString SaveSlotName = TEXT("SaveSlot_01");
};
