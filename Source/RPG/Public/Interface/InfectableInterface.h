// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InfectableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInfectableInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class RPG_API IInfectableInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    // 메시 컴포넌트 반환
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Infection")
    USkeletalMeshComponent* GetMeshComponent();

    // 원본 머티리얼 배열 반환
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Infection")
    TArray<UMaterialInstanceDynamic*> GetOriginalDynamicMaterials();

    // ← 새로 추가: 현재 머티리얼 설정
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SetCurrentMaterial(UMaterialInstanceDynamic* Material);
};
