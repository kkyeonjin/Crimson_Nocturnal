#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BaseCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FBaseCharacterStat : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentHP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ATK;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DEF;

};