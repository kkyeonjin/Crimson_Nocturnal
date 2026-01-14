// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Types/BaseStat.h"
#include "BossStat.generated.h"

USTRUCT(BlueprintType)
struct FBossStat : public FBaseStat
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ATK2 = 0; //Fire Storm Damage

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ATK3 = 0; //Slam Damage
};
