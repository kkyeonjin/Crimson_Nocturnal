// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/TitleGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

    FString LevelName = GetWorld()->GetMapName();

    // BGM Àç»ý¸¸!
    if (LevelName.Contains(TEXT("Title")))
    {
        if (UAudioComponent* AudioComp = UGameplayStatics::SpawnSound2D(this, TitleBGM))
        {
            AudioComp->Play();
        }
    }
}
