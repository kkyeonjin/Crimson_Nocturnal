// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BGMManager.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UBGMManager::PlayBGM(USoundCue* BGMCue)
{
    if (!BGMCue || !GetWorld()) return;  // World 확인 추가

    StopBGM();  // 기존 중지

    CurrentBGMAudioComp = UGameplayStatics::SpawnSound2D(GetWorld(), BGMCue, 0.7f);
    if (CurrentBGMAudioComp)
    {
        CurrentBGMAudioComp->bAllowSpatialization = false;
        UE_LOG(LogTemp, Log, TEXT("BGM 재생: %s"), *BGMCue->GetName());
    }
}

void UBGMManager::StopBGM()
{
    if (CurrentBGMAudioComp)
    {
        CurrentBGMAudioComp->Stop();
        CurrentBGMAudioComp->DestroyComponent();
        CurrentBGMAudioComp = nullptr;
    }
}
