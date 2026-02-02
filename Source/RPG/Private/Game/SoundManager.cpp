// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SoundManager.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void USoundManager::Init()
{
    // BGM용 오디오 컴포넌트 생성
    if (!BGMAudioComponent)
    {
        BGMAudioComponent = NewObject<UAudioComponent>();
        if (BGMAudioComponent)
        {
            BGMAudioComponent->RegisterComponent();
            BGMAudioComponent->bAutoActivate = false;
        }
    }
}

void USoundManager::PlayBGM(USoundCue* BGMCue)
{
    if (!BGMAudioComponent || !BGMCue) return;

    BGMAudioComponent->Stop();
    BGMAudioComponent->SetSound(BGMCue);
    BGMAudioComponent->Play();
}

void USoundManager::StopBGM()
{
    if (BGMAudioComponent)
    {
        BGMAudioComponent->Stop();
    }
}

void USoundManager::PlaySFX(USoundCue* SFXCue, FVector Location)
{
    if (!SFXCue) return;

    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SFXCue, Location);
}

