// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillTreeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillTreeWidget::ChangeSelection(int32 Input)
{
	if (CurrentSection == 0)
	{
		UltSelectCount -= Input;
		if (UltSelectCount  == -1)
		{
			UltSelectCount = 2;
		}
		else if (UltSelectCount == ULTNUM)
		{
			UltSelectCount = 0;
		}

		UE_LOG(LogTemp, Warning, TEXT("Count: %d"), UltSelectCount);

		UltCircles[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);
		UltDescriptions[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < ULTNUM; i++)
		{
			if (i != UltSelectCount)
			{
				UltCircles[i]->SetVisibility(ESlateVisibility::Hidden);
				UltDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else if (CurrentSection == 1)
	{
		SkillSelectCount -= Input;
		if (SkillSelectCount == -1)
		{
			SkillSelectCount = 4;
		}
		else if (SkillSelectCount == SKILLNUM)
		{
			SkillSelectCount = 0;
		}

		UE_LOG(LogTemp, Warning, TEXT("Count: %d"), SkillSelectCount);

		SkillCircles[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);
		SkillDescriptions[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < SKILLNUM; i++)
		{
			if (i != SkillSelectCount)
			{
				SkillCircles[i]->SetVisibility(ESlateVisibility::Hidden);
				SkillDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	
}

void USkillTreeWidget::SwitchSection()
{
	if (CurrentSection == 0)
	{
		CurrentSection = 1;
		SkillSelectCount = 0;

		UltCircles[UltSelectCount]->SetVisibility(ESlateVisibility::Hidden);
		UltDescriptions[UltSelectCount]->SetVisibility(ESlateVisibility::Hidden);

		SkillCircles[0]->SetVisibility(ESlateVisibility::Visible);
		SkillDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		CurrentSection = 0;
		UltSelectCount = 0;

		SkillCircles[SkillSelectCount]->SetVisibility(ESlateVisibility::Hidden);
		SkillDescriptions[SkillSelectCount]->SetVisibility(ESlateVisibility::Hidden);

		UltCircles[0]->SetVisibility(ESlateVisibility::Visible);
		UltDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillTreeWidget::InitializeUI()
{
	UltSelectCount = 0;
	UltCircles[0]->SetVisibility(ESlateVisibility::Visible);
	UltDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	for (int i = 1; i < ULTNUM; i++)
	{
		UltCircles[i]->SetVisibility(ESlateVisibility::Hidden);
		UltDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int i = 0; i < SKILLNUM; i++)
	{
		SkillCircles[i]->SetVisibility(ESlateVisibility::Hidden);
		SkillDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
	}

}

void USkillTreeWidget::SetSkillTree()
{
	if (CurrentSection == 0) //Ult
	{
		UltActivated[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);
		UltEquipped[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);
		for (int i = 0; i < ULTNUM; i++)
		{
			if (i != UltSelectCount)
			{
				UltActivated[i]->SetVisibility(ESlateVisibility::Hidden);
				UltEquipped[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}

		SkillTree->SetVisibility(ESlateVisibility::Visible);
		SkillTreeGrey->SetVisibility(ESlateVisibility::Hidden);
	}
	else //Skill
	{
		SkillActivated[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillTreeWidget::SetSkillPointText(int32 SkillPoint)
{
	if (SkillPointText)
	{
		SkillPointText->SetText(FText::AsNumber(SkillPoint));
	}
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UltCircles = { Ult1Circle, Ult2Circle, Ult3Circle }; 
	UltActivated = { Ult1Activated, Ult2Activated, Ult3Activated };
	UltDescriptions = { Ult1Description, Ult2Description, Ult3Description };
	UltEquipped = { Ult1Equipped, Ult2Equipped, Ult3Equipped };

	SkillCircles = { Skill1Circle, Skill2Circle, Skill3Circle, Skill4Circle, Skill5Circle };
	SkillDescriptions = { Skill1Description, Skill2Description, Skill3Description, Skill4Description, Skill5Description };
	SkillActivated = { Skill1Activated, Skill2Activated, Skill3Activated, Skill4Activated, Skill5Activated };


}

