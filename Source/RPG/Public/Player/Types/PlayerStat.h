#pragma once

#include "CoreMinimal.h"
#include "Types/BaseCharacterStat.h"
#include "PlayerStat.generated.h"

USTRUCT()
struct FPlayerStat : public FBaseCharacterStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxBp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentBp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EXP;
	
};
