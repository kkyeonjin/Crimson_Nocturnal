// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/DecalComponent.h"
#include "MagicCircleAoE.generated.h"

class USphereComponent;

UCLASS()
class RPG_API AMagicCircleAoE : public AActor
{
    GENERATED_BODY()

public:

    // Sets default values for this actor's properties
    AMagicCircleAoE();

    UFUNCTION(BlueprintCallable)
    void InitializeCircle();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // 마법진 Decal
    UPROPERTY(EditAnywhere, Category = "VFX")
    TSubclassOf<AActor> DecalClass;

    // 폭발 파티클 (에디터에서 설정)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<AActor> ParticleEffectClass;

    // 경고 시간
    UPROPERTY(EditAnywhere, Category = "Settings")
    float WarningDuration = 2.0f;

    // 경고 시간
    UPROPERTY(EditAnywhere, Category = "Settings")
    float ExplosionDuration = 2.0f;

    //보스 스테이지 센터(위치 랜덤 배정할 때의 기준점) 
    UPROPERTY(EditAnywhere, Category = "Settings")
    FVector StageCenter;

private:
    FTimerHandle ExplodeTimer;

    FTimerHandle WarningTimerHandle;

    void OnWarningEnd();

    void OnExplosionEnd();

    void SetRandomPoints();

    TArray<FVector> RandomPoints;

    FTimerHandle PulseTimer;
    float PulseAlpha = 0.0f;
};
