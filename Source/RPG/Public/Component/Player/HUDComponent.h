// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/SkillTreeWidget.h"
#include "Character/Player/Types/SkillTypes.h"
#include "HUDComponent.generated.h"

struct FInputActionValue;
class UMenuWidget;
class USkillTreeWidget;
class UMainHUDWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerSkillConfirmed, ESkillConfirmType, ConfirmType, int32, Index);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHUDComponent();

	UPROPERTY()
	FOnPlayerSkillConfirmed OnPlayerSkillConfirmed;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;  // 블루프린트 클래스를 지정

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillTreeWidget> SkillTreeUIClass;  // 블루프린트 클래스를 지정

	//Show / Hide MainHUDWidget
	

	//Show / Hide MenuWidget
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMenuWidget> MenuUIClass;  // 블루프린트 클래스를 지정

	UFUNCTION()
	void ShowMenuUI();

	UFUNCTION()
	void CloseMenuUI();

	UFUNCTION()
	void UpdatePlayerHP(int32 Current, int32 Max);

	UFUNCTION()
	void UpdatePlayerBP(int32 Current, int32 Max);

	UFUNCTION()
	void UpdateLevelText(int32 CurrentLevel);

	UFUNCTION()
	void ShowSkillTreeUI();

	UFUNCTION()
	void CloseSkillTreeUI();

	UFUNCTION()
	void SetSkillTree(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting);

	UFUNCTION()
	void SetSkillPointText(int32 SkillPoint);

	void InitializeSkillTreeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills);

	void ShowReadyToUseUlt();

	// UI 표시 + 자동 숨김 처리

	void ShowLevelUpPopUp(int32 CurrentLevel);

	void HideLevelUp();

	void HidePopUp();

	//Start PopUp
	void ShowStartPopUp();

	//Combo UI
	void ShowComboPopUp(int32 ComboNum);

	void HideComboPopUp();

	//EventUI
	void ShowQTEUI(int32 ComboNum);

	void HideQTEUI(int32 ComboNum);

	//Break the Crystal
	void NoticeToBreak();


	//Skill Screen Effect
	void ShowSkillEffect(int32 SkillNum);

	void HideSkillEffect();

	//Interact PopUp
	void ShowInteractPopUp();

	void HideInteractPopUp();

	//Dance PopUP
	void ShowDancePopUp();

	//Show Dialogue
	UFUNCTION()
	void ShowDialogue(const FString& DialogueName, const FString& DialogueLine);

	void HideDialogue();

	FORCEINLINE int32 GetUltSelectCount() { return SkillTreeUIWidget->GetUltSelectCount(); }
	FORCEINLINE int32 GetSkillSelectCount() { return SkillTreeUIWidget->GetSkillSelectCount(); }
	FORCEINLINE int32 GetCurrentSection() { return SkillTreeUIWidget->GetCurrentSection(); }
	FORCEINLINE int32 GetXorYSettingCount() { return SkillTreeUIWidget->GetXorYSettingCount(); }

	FORCEINLINE USkillTreeWidget* GetSkillTreeUI() { return SkillTreeUIWidget; };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UMainHUDWidget* MainHUDWidget;

	UPROPERTY()
	USkillTreeWidget* SkillTreeUIWidget;

	UPROPERTY()
	UMenuWidget* MenuUIWidget;

private:
	UFUNCTION()
	void HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index); //Callback Func

	FTimerHandle ComboPopUpTimer;

	FTimerHandle LevelUpPopUpTimer;

	FTimerHandle SkillEffectTimer;

};
