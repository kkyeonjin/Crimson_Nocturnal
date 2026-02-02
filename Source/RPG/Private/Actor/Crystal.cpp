// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Crystal.h"

// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACrystal::GetHit(const FVector& ImpactPoint, AActor* Target)
{
	if (bIsBreakable)
	{
		DestroyLastCrystal();
	}
}

void ACrystal::DestroyCrystal(int32 CrystalToBreak)
{
	if (CrystalNum == CrystalToBreak)
	{
		//PlayVFX;
		//PlaySound();

		Destroy();
	}
}

void ACrystal::DestroyLastCrystal()
{
	//PlayVFX();
	//PlaySound();

	//ownerBoss->PlayAnimation
	OnLastCrystalDestroyed.Broadcast();
	
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
}



