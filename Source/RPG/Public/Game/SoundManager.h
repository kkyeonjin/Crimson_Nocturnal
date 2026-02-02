// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SoundManager.generated.h"

class USoundCue;
class UAudioComponent;
/**
 * 
 */
UCLASS()
class RPG_API USoundManager : public UObject
{
	GENERATED_BODY()
	
public:
    void Init();

    // BGM 관련
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    UAudioComponent* BGMAudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundCue* TitleBGM;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundCue* GameplayBGM;

    // SFX 사운드 큐들
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundCue* PlayerAttackSFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundCue* PlayerHitSFX;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
    USoundCue* BossAttackSFX;

    // 기능
    void PlayBGM(USoundCue* BGMCue);
    void StopBGM();
    void PlaySFX(USoundCue* SFXCue, FVector Location = FVector::ZeroVector);

};
