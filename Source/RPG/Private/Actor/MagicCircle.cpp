// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MagicCircle.h"

#include "Components/DecalComponent.h"

// Sets default values
AMagicCircle::AMagicCircle()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

}

void AMagicCircle::InitializeDecal()
{
    Decal->SetVisibility(true);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            LifetimeHandle,
            this,
            &AMagicCircle::DestroyDecal,
            1.f,
            false
        );
    }
}

void AMagicCircle::SetDecal(UDecalComponent* MagicCircle)
{
    Decal = MagicCircle;

    Decal->SetVisibility(false);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            FadeOutTimerHandle,
            this,
            &AMagicCircle::InitializeDecal,
            1.f,
            false
        );
    }

}

void AMagicCircle::DestroyDecal()
{
    Destroy();
}

// Called when the game starts or when spawned
void AMagicCircle::BeginPlay()
{
    Super::BeginPlay();
}

