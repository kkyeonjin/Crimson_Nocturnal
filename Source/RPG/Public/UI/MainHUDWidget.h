// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class RPG_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* BPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BPText;

	UFUNCTION(BlueprintCallable)
	void UpdateHP(int32 Current, int32 Max);

	UFUNCTION(BlueprintCallable)
	void UpdateBP(int32 Current, int32 Max);
};
