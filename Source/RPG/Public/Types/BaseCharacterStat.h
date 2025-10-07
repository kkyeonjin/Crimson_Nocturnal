#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FBaseCharacterStat : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ATK;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DEF;

};