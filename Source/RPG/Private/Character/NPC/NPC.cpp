// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NPC.h"
#include "Component/NPC/DialogueComponent.h"
#include "Game/RPGGameInstance.h"
#include "Game/RPGGameMode.h"

ANPC::ANPC()
{
    PrimaryActorTick.bCanEverTick = false;

}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

    DialogueComponent = FindComponentByClass<UDialogueComponent>();

}

void ANPC::OnInteract()
{
    //대화
    if (DialogueComponent)
    {
        FString DialogueName = DialogueComponent->GetDialogueName();
        FString DialogueLine = DialogueComponent->GetDialogueLine();

        if (ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode()))
        {
            GM->BroadcastDialogue(DialogueName, DialogueLine);
        }
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Dialog in NPC NOT FOUND!!!!"));
    }
}

