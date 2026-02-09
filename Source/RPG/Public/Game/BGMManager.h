// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundCue.h"
#include "BGMManager.generated.h"

class UAudioComponent;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, EditInlineNew)
class RPG_API UBGMManager : public UObject
{
	GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|BGM")
    USoundCue* HospitalBGM;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|BGM")
    USoundCue* NecropolisBGM;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|BGM")
    USoundCue* CathedralBGM;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|BGM")
    USoundCue* BossBGM;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio|BGM")
    USoundCue* SaloonBGM;

    void PlayBGM(USoundCue* BGMCue);
    void StopBGM();
	
private:
    UPROPERTY()
    UAudioComponent* BGMAudioComponent;
    UPROPERTY()
    USoundCue* CurrentBGM;

};
