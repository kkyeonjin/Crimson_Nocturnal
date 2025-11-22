// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelData.generated.h"

USTRUCT()
struct FLevelData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 RequiredEXP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefaultATK;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefaultDEF;

};
