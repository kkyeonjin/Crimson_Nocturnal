# pragma once

#include "CoreMinimal.h"
#include "Character/Enemy/Types/BaseStat.h"
#include "EnemyStat.generated.h"

USTRUCT()
struct FEnemyStat : public FBaseStat
{
	GENERATED_BODY()

	//EXP that Player gain when enemy killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EXPValue = 0;

};
