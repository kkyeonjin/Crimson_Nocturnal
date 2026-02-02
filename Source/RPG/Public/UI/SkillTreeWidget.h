// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define ULTNUM  3
#define SKILLNUM  5

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/Types/SkillTypes.h"
#include "SkillTreeWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillConfirmed, ESkillConfirmType, ConfirmType, int32, Index);

class UTextBlock;
class UImage;

/**
 *
 */
UCLASS()
class RPG_API USkillTreeWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	void ChangeSelection(int32 Input);
	void SwitchSection();
	void InitializeSelection();
	void SetSkillTree();
	void SetSkillPointText(int32 SkillPoint);

	//OnLevelUp
	void ShowLevelUpPopUp();

	void InitializeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills);

	void ReadyToSkillASetting(int32 Input);
	void ChangeSkillASettingSelection(int32 Input);

	void OnDirectionalInput(int32 Input);

	UPROPERTY()
	FOnSkillConfirmed OnSkillConfirmed;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill1Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill2Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill3Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill4Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill5Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill1Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill2Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill3Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill4Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill5Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill1Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill2Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill3Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill4Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill5Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult1Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult2Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult3Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult1Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult2Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult3Activated;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillTree;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillTreeGrey;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult1Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult2Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult3Description;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult1Symbol;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult2Symbol;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult3Symbol;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillPointText;

	UPROPERTY(meta = (BindWidget))
	UImage* XSettingHighlight;

	UPROPERTY(meta = (BindWidget))
	UImage* YSettingHighlight;

	UPROPERTY(meta = (BindWidget))
	UImage* X1;

	UPROPERTY(meta = (BindWidget))
	UImage* X2;

	UPROPERTY(meta = (BindWidget))
	UImage* X3;

	UPROPERTY(meta = (BindWidget))
	UImage* X4;

	UPROPERTY(meta = (BindWidget))
	UImage* Y1;

	UPROPERTY(meta = (BindWidget))
	UImage* Y2;

	UPROPERTY(meta = (BindWidget))
	UImage* Y3;

	UPROPERTY(meta = (BindWidget))
	UImage* Y4;

	UPROPERTY(meta = (BindWidget))
	UImage* Y1Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Y2Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Y3Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* Y4Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* X1Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* X2Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* X3Highlight;

	UPROPERTY(meta = (BindWidget))
	UImage* X4Highlight;


	FORCEINLINE int32 GetSkillSelectCount() { return SkillSelectCount; }
	FORCEINLINE int32 GetUltSelectCount() { return UltSelectCount; }
	FORCEINLINE int32 GetCurrentSection() { return CurrentSection; }
	int32 GetXorYSettingCount();


private:
	int32 UltSelectCount = 0;
	int32 CurrentSection = 0; //default 0:Ult, 1:Skill, 2:SkillA Setting 
	TArray<UImage*> UltHighlights;
	TArray<UImage*> UltActivated;
	TArray<UImage*> UltDescriptions;
	TArray<UImage*> UltSymbols;

	int32 SkillSelectCount = 0;
	TArray<UImage*> SkillHighlights;
	TArray<UImage*> SkillDescriptions;
	TArray<UImage*> SkillActivated;

	int32 XSettingCount = 0;
	int32 YSettingCount = 0;
	TArray<UImage*> XSettingNums;
	TArray<UImage*> YSettingNums;
	TArray<UImage*> XSettingHighlights;
	TArray<UImage*> YSettingHighlights;
};
