// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Portal.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/NPC/NPC.h"

APortal::APortal()
{
    PrimaryActorTick.bCanEverTick = false;

    //아래의 항목은 전부 에디터 블루프린트에서 설정해주는걸로!!!
    /*
    // Collision Box
    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    SetRootComponent(CollisionBox);
    CollisionBox->SetBoxExtent(FVector(200, 200, 300));
    CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
    CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    // Portal Mesh
    PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PortalMesh"));
    PortalMesh->SetupAttachment(CollisionBox);

    // ⭐ 태그 설정
    Tags.Add(FName("Portal"));
	*/

}

void APortal::OnInteract()
{
    //Move to Next Map
    if (!DestinationMap.IsNone())
    {
        UGameplayStatics::OpenLevel(GetWorld(), DestinationMap);
    }
}

