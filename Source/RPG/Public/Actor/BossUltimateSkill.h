// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossUltimateSkill.generated.h"

class APlayerCharacter;

UCLASS()
class RPG_API ABossUltimateSkill : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABossUltimateSkill();

	void StartUltSkill();

	UFUNCTION(BlueprintCallable, Category = "Ultimate|Mechanic")
	void TeleportPlayerToCenter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ultimate|References")
	APlayerCharacter* TargetPlayer = nullptr;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ultimate|Settings", meta = (AllowPrivateAccess))
	float TeleportHeight = 100.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DarkStormActorClass;

	FTimerHandle StormTimer;

public:
	FORCEINLINE void SetTargetPlayer(APlayerCharacter* NewPlayer) { TargetPlayer = NewPlayer; }


};
