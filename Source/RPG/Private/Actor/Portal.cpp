// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Portal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/NPC/NPC.h"
#include "Game/RPGGameInstance.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = false;

}

void APortal::OnInteract()
{
    //Move to Next Map
    if (!DestinationMap.IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), DestinationMap);

    }
}

