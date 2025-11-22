// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define ULTNUM  3
#define SKILLNUM  5

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillTreeWidget.generated.h"

class UTextBlock;
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
	void InitializeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills);

	UPROPERTY(meta = (BindWidget))
	class UImage* Skill1Circle;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill2Circle;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill3Circle;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill4Circle;

	UPROPERTY(meta = (BindWidget))
	UImage* Skill5Circle;

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
	UImage* Ult1Circle;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult2Circle;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult3Circle;

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
	UImage* Ult1Equipped;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult2Equipped;

	UPROPERTY(meta = (BindWidget))
	UImage* Ult3Equipped;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillPointText;

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




	FORCEINLINE int32 GetSkillSelectCount() { return SkillSelectCount; }
	FORCEINLINE int32 GetUltSelectCount() { return UltSelectCount; }
	FORCEINLINE int32 GetCurrentSection() { return CurrentSection; }


private:
	int32 UltSelectCount = 0;
	int32 CurrentSection = 0; //default 0:Ult, 1:Skill
	TArray<UImage*> UltCircles;
	TArray<UImage*> UltActivated;
	TArray<UImage*> UltDescriptions;
	TArray<UImage*> UltEquipped;

	int32 SkillSelectCount = 0;
	TArray<UImage*> SkillCircles;
	TArray<UImage*> SkillDescriptions;
	TArray<UImage*> SkillActivated;
};
