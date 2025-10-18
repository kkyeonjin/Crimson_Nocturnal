#pragma once

#include "CoreMinimal.h"
#include "Types/BaseCharacterStat.h"
#include "EnemyStat.generated.h"

USTRUCT()
struct FEnemyStat : public FBaseCharacterStat
{
	GENERATED_BODY()

	//EXP that Player gain when enemy killed
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EXPValue;

};
