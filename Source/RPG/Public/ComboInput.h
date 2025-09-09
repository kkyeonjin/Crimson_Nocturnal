#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "ComboInput.generated.h"

UENUM(BlueprintType)
enum class EAttackKeyType : uint8
{
	X, Y
};

USTRUCT(BlueprintType)
struct FComboPattern
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    TArray<EAttackKeyType> ComboKeySequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 ComboNum;

    //use soft pointer to avoid GC crash
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    TSoftObjectPtr<UAnimMontage> ComboAnimMontage;
};
