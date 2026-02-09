// Fill out your copyright notice in the Description page of Project Settings.

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
	// Sets default values for this actor's properties
	ACrystal();

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
