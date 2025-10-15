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

    //designate their own number 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 AnimMontageNum;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    float ComboAttackMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 BPValue;

};
