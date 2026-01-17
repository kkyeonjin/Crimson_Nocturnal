// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/NPC/NPC.h"

ANPC::ANPC()
{
    PrimaryActorTick.bCanEverTick = false;

    /*
    // Collision Box
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    RootComponent = CollisionBox;
    CollisionBox->SetBoxExtent(FVector(100, 100, 150));
    CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // ⭐ 태그 설정
    Tags.Add(FName("NPC"));

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANPC::OnCollisionBeginOverlap);
    CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANPC::OnCollisionEndOverlap);

    // 기본 NPC 설정
    NPCName = TEXT("NPC");
	*/
}

void ANPC::OnInteract()
{
	//대화
    UE_LOG(LogTemp, Display, TEXT("LOLOLOLOLOL"));
}

