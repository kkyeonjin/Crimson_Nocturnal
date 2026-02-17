// Manages all player UI: MainHUD, SkillTree, Menu, Popups, QTE
// Handles input mode switching, time dilation, auto-hide timers

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWidgetsReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerSkillConfirmed, ESkillConfirmType, ConfirmType, int32, Index);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHUDComponent();


	// Widget classes
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;  
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillTreeWidget> SkillTreeUIClass;  
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMenuWidget> MenuUIClass;  


	// Main HUD updates (from AttributeComponent)
	UFUNCTION()
	void UpdatePlayerHP(int32 Current, int32 Max);
	UFUNCTION()
	void UpdatePlayerBP(int32 Current, int32 Max);
	UFUNCTION()
	void UpdateLevelText(int32 CurrentLevel);
	void ShowReadyToUseUlt();


	// SkillTree
	UFUNCTION()
	void ShowSkillTreeUI();
	UFUNCTION()
	void CloseSkillTreeUI();
	UFUNCTION()
	void SetSkillTree(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting);
	UFUNCTION()
	void SetSkillPointText(int32 SkillPoint);
	void InitializeSkillTreeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills);


	// Menu
	UFUNCTION()
	void ShowMenuUI();
	UFUNCTION()
	void CloseMenuUI();


	// PopUp & Effects
	void ShowLevelUpPopUp(int32 CurrentLevel);
	void HideLevelUp();
	void HidePopUp();
	void ShowStartPopUp();
	void ShowComboPopUp(int32 ComboNum);
	void HideComboPopUp();
	void ShowSkillEffect(int32 SkillNum);
	void HideSkillEffect();

	//Interact PopUp
	void ShowInteractPopUp();
	void HideInteractPopUp();
	UFUNCTION()
	void ShowDialogue(const FString& DialogueName, const FString& DialogueLine);
	void HideDialogue();


	// Event/Gameplay UI
	void ShowQTEUI(int32 ComboNum);
	void HideQTEUI(int32 ComboNum);
	void NoticeToBreak();
	void ShowDancePopUp();


	// Delegate to PlayerCharacter
	UPROPERTY()
	FOnPlayerSkillConfirmed OnPlayerSkillConfirmed;

	FOnWidgetsReady OnWidgetsReady;


protected:
	virtual void BeginPlay() override;


private:
	// Cached widgets
	UPROPERTY()
	UMainHUDWidget* MainHUDWidget;
	UPROPERTY()
	USkillTreeWidget* SkillTreeUIWidget;
	UPROPERTY()
	UMenuWidget* MenuUIWidget;


	// Auto-hide timers
	FTimerHandle ComboPopUpTimer;
	FTimerHandle LevelUpPopUpTimer;
	FTimerHandle SkillEffectTimer;


	// Helpers
	UFUNCTION()
	void HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index); //Callback Func
	void SetupInputMode(APlayerController* PC, UUserWidget* WidgetToFocus, bool bGamePaused);


public:
	FORCEINLINE int32 GetUltSelectCount() { return SkillTreeUIWidget->GetUltSelectCount(); }
	FORCEINLINE int32 GetSkillSelectCount() { return SkillTreeUIWidget->GetSkillSelectCount(); }
	FORCEINLINE int32 GetCurrentSection() { return SkillTreeUIWidget->GetCurrentSection(); }
	FORCEINLINE int32 GetXorYSettingCount() { return SkillTreeUIWidget->GetXorYSettingCount(); }
	FORCEINLINE USkillTreeWidget* GetSkillTreeUI() { return SkillTreeUIWidget; };
};
