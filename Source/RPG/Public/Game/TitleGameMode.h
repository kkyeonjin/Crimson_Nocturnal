// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TitleGameMode.generated.h"

class USoundCue;
/**
 * 
 */
UCLASS()
class RPG_API ATitleGameMode : public AGameMode
{
	GENERATED_BODY()
	
protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "Audio")
    USoundCue* TitleBGM;  // BGM¸¸!

};
