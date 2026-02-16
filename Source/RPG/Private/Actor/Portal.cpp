// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Portal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/NPC/NPC.h"
#include "Game/RPGGameInstance.h"
#include "Game/RPGGameMode.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = false;

}

void APortal::OnInteract()
{
    if (!DestinationMap.IsNone())
    {
        FString LevelString = DestinationMap.ToString();

        ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode());

        GM->OnSaveRequired.Broadcast();

        if (EnteringSaloon)
        {
            LevelString += TEXT("#SaloonDoor");
            //Saloon 입장 이벤트
            GM->OnEnterSaloon.Broadcast();  // GameMode로 전달
            UE_LOG(LogTemp, Warning, TEXT("Enter Saloon!!! in portal"));
        }
        else if (ExitingSaloon)
        {
            //Saloon 퇴장 이벤트
            GM->OnExitSaloon.Broadcast();
        }

        UGameplayStatics::OpenLevel(GetWorld(), FName(*LevelString));

        UE_LOG(LogTemp, Warning, TEXT("Portal: %s → Saloon %s"),
            *GetName(), EnteringSaloon ? TEXT("Enter") : TEXT("Exit"));
    }
}

