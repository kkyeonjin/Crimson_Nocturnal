// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BGMManager.h"
#include "Components/AudioComponent.h"

void UBGMManager::PlayBGM(USoundCue* BGMCue)
{
    if (!BGMCue) return;

    // AudioComponent가 없으면 생성
    if (!BGMAudioComponent)
    {
        BGMAudioComponent = NewObject<UAudioComponent>();
        if (BGMAudioComponent)
        {
            BGMAudioComponent->RegisterComponent();
            BGMAudioComponent->SetVolumeMultiplier(0.7f);  // 음량 조절
        }
    }

    if (BGMAudioComponent)
    {
        BGMAudioComponent->Stop();
        BGMAudioComponent->SetSound(BGMCue);
        BGMAudioComponent->Play();
        CurrentBGM = BGMCue;

        UE_LOG(LogTemp, Log, TEXT("BGM 재생: %s"), *BGMCue->GetName());
    }
}

void UBGMManager::StopBGM()
{
    if (BGMAudioComponent)
    {
        BGMAudioComponent->Stop();
        CurrentBGM = nullptr;

        UE_LOG(LogTemp, Log, TEXT("BGM 정지"));
    }
}
