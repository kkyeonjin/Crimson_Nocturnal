// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Crystal.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Crystal%d BREAK!!"), CrystalNum);

		Destroy();

		PlayCrystalSFX();
		PlayCrystalVFX();
	}
}

void ACrystal::DestroyLastCrystal()
{
	PlayCrystalSFX();
	PlayCrystalVFX();

	//ownerBoss->PlayAnimation
	OnLastCrystalDestroyed.Broadcast();
	
}

void ACrystal::PlayCrystalSFX()
{
	if (BreakParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BreakParticles,
			GetActorLocation()
		);
	}
}

void ACrystal::PlayCrystalVFX()
{
	FVector ParticleLocation = GetActorLocation() + FVector(0, 0, 20);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BreakSFX, GetActorLocation());
}

void ACrystal::SetReadyToBreak()
{
	if (CrystalNum == 3)
	{
		bIsBreakable = true;
	}
}



