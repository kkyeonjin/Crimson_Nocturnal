// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerStatDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FPlayerStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 MaxHp;

	UPROPERTY(EditAnywhere)
	int32 MaxBp;

	UPROPERTY(EditAnywhere)
	int32 CurrentHp;

	UPROPERTY(EditAnywhere)
	int32 CurrentBp;

	
};


/**
 * 
 */
UCLASS()
class RPG_API UPlayerStatDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FPlayerStats PlayerStats;

	UPlayerStatDataAsset();
};
