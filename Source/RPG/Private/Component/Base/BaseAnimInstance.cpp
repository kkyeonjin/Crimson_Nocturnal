// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Base/BaseAnimInstance.h"


void UBaseAnimInstance::PlayMontageByIndex(int32 Index)
{
    UAnimMontage* Montage = GetMontageAtIndex(Index);
    if (Montage)
    {
        Montage_Play(Montage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Montage at index %d not found"), Index);
    }
}

void UBaseAnimInstance::StopAllMontages()
{
}
