// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/SkillTreeWidget.h"
#include "Character/Player/Types/SkillTypes.h"
#include "HUDComponent.generated.h"

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

	void UpdatePlayerHP(int32 Current, int32 Max);

	void UpdatePlayerBP(int32 Current, int32 Max);

	void ShowSkillTreeUI();

	void HideSkillTreeUI();

	void ChangeSelection(int32 Input);

	void SwitchSection();

	void HandleSkillTreeNavigation(int32 Input);

	void SetSkillTree();

	void SetSkillPointText(int32 SkillPoint);

	void InitializeSkillTreeSelection();

	void InitializeSkillTreeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills);

	void ReadyToSkillASetting(int32 Input);

	void ChangeSkillASettingSelection(int32 Input);

	// UI 표시 + 자동 숨김 처리

	void ShowLevelUpPopUp(int32 CurrentLevel);

	void HideLevelUpPopUp();

	//Combo UI
	void ShowComboPopUp(int32 ComboNum);

	void HideComboPopUp();

	//EventUI

	void ShowQTEUI(int32 ComboNum);

	void HideQTEUI(int32 ComboNum);

	//Skill Screen Effect
	void ShowSkillEffect(int32 SkillNum);

	void HideSkillEffect();

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

private:
	UFUNCTION()
	void HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index); //Callback Func

	FTimerHandle ComboPopUpTimer;

	FTimerHandle LevelUpPopUpTimer;

	FTimerHandle SkillEffectTimer;

};
