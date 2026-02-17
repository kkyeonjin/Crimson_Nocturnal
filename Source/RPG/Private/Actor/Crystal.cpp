#include "Actor/Crystal.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

ACrystal::ACrystal()
{
 	PrimaryActorTick.bCanEverTick = false;
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
		Destroy();

		PlayCrystalSFX();
		PlayCrystalVFX();
	}
}

void ACrystal::DestroyLastCrystal()
{
	PlayCrystalSFX();
	PlayCrystalVFX();

	Destroy();

	// delegate to owner(Boss)
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
	// Only when crystal is breakable (can be hit by player's attack)
	if (CrystalNum == 3)
	{
		bIsBreakable = true;
	}
}



