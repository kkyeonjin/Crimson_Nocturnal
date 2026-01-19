// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class RPG_API AMagicCircle : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMagicCircle();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UDecalComponent* Decal;

    // 초기화 함수
    UFUNCTION(BlueprintCallable, Category = "Magic Circle")
    void InitializeDecal();

    UFUNCTION(BlueprintCallable, Category = "Magic Circle")
    void SetDecal(UDecalComponent* MagicCircle);

private:
    FTimerHandle LifetimeHandle;
    FTimerHandle FadeOutTimerHandle;

    // ? 페이드 관련 변수
    float CurrentAlpha = 0.0f;
    float TargetAlpha = 1.0f;
    float FadeSpeed = 0.0f;
    bool bIsFading = false;

    void DestroyDecal();
    //void StartFadeOut();
    //void UpdateFade();
};

