// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerRuntimeStat.generated.h"

USTRUCT(BlueprintType)
struct FPlayerRuntimeStat
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentLevel = 1;

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentExp = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentHP = 100;

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentBP = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 MaxHP = 100;

    UPROPERTY(BlueprintReadWrite)
    int32 MaxBP = 100;

    UPROPERTY(BlueprintReadWrite)
    int32 ATK = 10;

    UPROPERTY(BlueprintReadWrite)
    int32 DEF = 5;

    UPROPERTY(BlueprintReadWrite)
    int32 SkillPoint = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 EquippedUlt = 0;

    UPROPERTY(BlueprintReadWrite)
    TArray<int32> ActivatedSkills;

    UPROPERTY(BlueprintReadWrite)
    FString CurrentMap;

    UPROPERTY(BlueprintReadWrite)
    int32 Ult1XSetting = 0;

    UPROPERTY(BlueprintReadWrite)
    int32 Ult1YSetting = 0;

    FPlayerRuntimeStat() { ActivatedSkills.Init(0, 5); }
};
