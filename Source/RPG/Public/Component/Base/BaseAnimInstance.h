// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    // ｻﾓ ﾅｬｷ｡ｽｺｿ｡ｼｭ ｻ鄙・?ｰ・ﾇﾔｼ・
    UFUNCTION(BlueprintCallable)
    void PlayMontageByIndex(int32 Index);

    UFUNCTION(BlueprintCallable)
    void StopAllMontages();

protected:
    // ｻﾓ ﾅｬｷ｡ｽｺｿ｡ｼｭ overrideﾇﾒ ﾇﾔｼ・
    virtual UAnimMontage* GetMontageAtIndex(int32 Index) { return nullptr; }
    virtual int32 GetMaxMontageIndex() { return 0; }
	
};
