// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UImage;
class UTextBlock;
/**
 *
 */
UCLASS()
class RPG_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	//MainHUDPanel
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

	//EventUIPanel
	UPROPERTY(meta = (BindWidget))
	UImage* ComboA;

	UPROPERTY(meta = (BindWidget))
	UImage* ComboB;

	UPROPERTY(meta = (BindWidget))
	UImage* ComboC;

	UPROPERTY(meta = (BindWidget))
	UImage* ComboD;

	UFUNCTION()
	void ShowQTEUI(int32 ComboNum);

	UFUNCTION()
	void HideQTEUI(int32 ComboNum);


private:
	TArray<UImage*> ComboQTEImages;

};
