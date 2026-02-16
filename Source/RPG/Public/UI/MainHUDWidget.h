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

	//Sub Widget
	UPROPERTY(EditAnywhere)
	class USkillScreenOverlayWidget* SkillScreenOverlayWidget;

	UFUNCTION()
	void OnSkillStarted(int32 SkillNum);

	UFUNCTION()
	void OnSkillFinished();

	//MainHUDPanel
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* BPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* BPText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelText;

	UFUNCTION(BlueprintCallable)
	void UpdateHP(int32 Current, int32 Max);

	UFUNCTION(BlueprintCallable)
	void UpdateBP(int32 Current, int32 Max);

	UFUNCTION()
	void UpdateLevelText(int32 CurrentLevel);

	UFUNCTION()
	void SetUltEmblem(int32 UltNum);

	void ShowReadyToUseUlt();

	//EventUIPanel
	UPROPERTY(meta = (BindWidget))
	UImage* ComboA;

	UPROPERTY(meta = (BindWidget))
	UImage* ComboB;

	UPROPERTY(meta = (BindWidget))
	UImage* ComboC;

	UPROPERTY(meta = (BindWidget))
	UImage* ComboD;

	UPROPERTY(meta = (BindWidget))
	UImage* UltA;

	UPROPERTY(meta = (BindWidget))
	UImage* UltB;

	UPROPERTY(meta = (BindWidget))
	UImage* UltC;

	UPROPERTY(meta = (BindWidget))
	UImage* UltAReady;

	UPROPERTY(meta = (BindWidget))
	UImage* UltBReady;

	UPROPERTY(meta = (BindWidget))
	UImage* UltCReady;

	UPROPERTY(meta = (BindWidget))
	UImage* LevelUp;

	UPROPERTY(meta = (BindWidget))
	UImage* Level2PopUp;

	UPROPERTY(meta = (BindWidget))
	UImage* Level5PopUp;

	UPROPERTY(meta = (BindWidget))
	UImage* StartPopUp;

	UPROPERTY(meta = (BindWidget))
	UImage* BreakTheCrystal;

	UPROPERTY(meta = (BindWidget))
	UImage* InteractPopUp;

	UPROPERTY(meta = (BindWidget))
	UImage* DancePopUp;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* DialogueName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueLine;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* DialoguePanel;

	UFUNCTION()
	void ShowQTEUI(int32 ComboNum);

	UFUNCTION()
	void HideQTEUI(int32 ComboNum);

	UFUNCTION()
	void ShowLevelUpPopUp(int32 CurrentLevel);

	UFUNCTION()
	void HideLevelUp();

	UFUNCTION()
	void HidePopUp();

	UFUNCTION()
	void ShowStartPopUp();

	UFUNCTION()
	void ShowComboPopUp(int32 ComboNum);

	UFUNCTION()
	void HideComboPopUp();

	UFUNCTION()
	void NoticeToBreak();

	UFUNCTION()
	void ShowInteractPopUp();

	UFUNCTION()
	void HideInteractPopUp();

	UFUNCTION()
	void ShowDialogue(const FString& NewDialogueName, const FString& NewDialogueLine);

	UFUNCTION()
	void HideDialogue();

	void ShowDancePopUp();

	UFUNCTION(BlueprintCallable)
	void SetSubWidgets(USkillScreenOverlayWidget* Widget);


private:
	TArray<UImage*> ComboQTEImages;

	TArray<UImage*> ComboPopUpImages;

	TArray<UImage*> UltImages;

	TArray<UImage*> UltReadyImages;

	UImage* SpecialLevelUp;

	//for Ult Emblem Initialize 
	bool IsReadyToUseUlt = false;

};
