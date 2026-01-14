#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseStat.generated.h"

USTRUCT(BlueprintType)
struct FBaseStat : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHP = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentHP = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ATK = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DEF = 0;

};
