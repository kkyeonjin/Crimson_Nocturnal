// Crystal that around the Boss (3 crystals)
// each crystal is broke according to the boss's HP (2/3, 1/3, 0(when dead))

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "Crystal.generated.h"

class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLastCrystalDestroyed);

UCLASS()
class RPG_API ACrystal : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ACrystal();

	// IHitInterface cause last crystal to break(in the middle) is hittable & interactable with player
	virtual void GetHit(const FVector& ImpactPoint, AActor* Target) override;

	void DestroyCrystal(int32 CrystalToBreak);

	void SetReadyToBreak();

	FOnLastCrystalDestroyed OnLastCrystalDestroyed;

protected:
	void DestroyLastCrystal();

private:

	void PlayCrystalSFX();
	void PlayCrystalVFX();

	//Set in Engine Editor 
	UPROPERTY(EditAnywhere)
	bool bIsBreakable;

	UPROPERTY(EditAnywhere)
	int32 CrystalNum;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BreakParticles;

	UPROPERTY(EditAnywhere)
	USoundCue* BreakSFX;

};
