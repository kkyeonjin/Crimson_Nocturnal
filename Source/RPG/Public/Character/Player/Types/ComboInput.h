// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimMontage.h"
#include "ComboInput.generated.h"

// 콤보 이름을 Enum으로 정의
UENUM(BlueprintType)
enum class EComboType : uint8
{
    // X-X-Y 콤보
    PowerStrike UMETA(DisplayName = "Power Strike (X-X-Y)"),

    // X-Y-X 콤보
    SpinAttack UMETA(DisplayName = "Spin Attack (X-Y-X)"),

    // X-Y-Y-X 콤보
    AdvancedCombo UMETA(DisplayName = "Advanced (X-Y-Y-X)"),

    // Y-X-Y-Y 콤보
    UltimateCombo UMETA(DisplayName = "Ultimate (Y-X-Y-Y)"),

    MAX_COMBOS
};

// 애니메이션 몬타주 인덱스를 Enum으로
UENUM(BlueprintType)
enum class EAnimMontageType : uint8
{
    ComboA = 0 UMETA(DisplayName = "Combo A Animation"),
    ComboB = 1 UMETA(DisplayName = "Combo B Animation"),
    ComboC = 2 UMETA(DisplayName = "Combo C Animation"),
    ComboD = 3 UMETA(DisplayName = "Combo D Animation"),
    //Default = 0
};

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
    EComboType ComboType;  // 이제 의미가 명확

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    TArray<EAttackKeyType> ComboKeySequence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 ComboNum = 0;

    //designate their own number 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 AnimMontageNum = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    float ComboAttackMultiplier = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combo")
    int32 BPValue = 0;

};

//
namespace ComboConstants
{
    // chain attack multipliers
    constexpr float CHAIN_X1_MULTIPLIER = 1.0f;
    constexpr float CHAIN_X2_MULTIPLIER = 1.2f;
    constexpr float CHAIN_X3_MULTIPLIER = 1.5f;

    constexpr float CHAIN_Y1_MULTIPLIER = 1.0f;
    constexpr float CHAIN_Y2_MULTIPLIER = 1.3f;

    // timeout values
    constexpr float COMBO_TIMEOUT = 2.0f;
    constexpr float BOSS_SURVIVAL_TIMEOUT = 8.0f;
}



