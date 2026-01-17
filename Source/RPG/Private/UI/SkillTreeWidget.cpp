// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SkillTreeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillTreeWidget::ChangeSelection(int32 Input) //Up&Down Button
{
	if (CurrentSection == static_cast<int32>(ESkillTreeSection::UltimateSkill))
	{
		UltSelectCount -= Input;
		if (UltSelectCount == -1)
		{
			UltSelectCount = 2;
		}
		else if (UltSelectCount == ULTNUM)
		{
			UltSelectCount = 0;
		}

		UE_LOG(LogTemp, Warning, TEXT("Count: %d"), UltSelectCount);

		UltHighlights[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);
		UltDescriptions[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < ULTNUM; i++)
		{
			if (i != UltSelectCount)
			{
				UltHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
				UltDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else if (CurrentSection == static_cast<int32>(ESkillTreeSection::NormalSkill))
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

		SkillHighlights[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);
		SkillDescriptions[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < SKILLNUM; i++)
		{
			if (i != SkillSelectCount)
			{
				SkillHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
				SkillDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}

}

//L&R Button when SkillA Key Setting
void USkillTreeWidget::ChangeSkillASettingSelection(int32 Input)
{
	//X Setting
	if (XSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
	{
		XSettingHighlights[XSettingCount]->SetVisibility(ESlateVisibility::Hidden);
		XSettingCount += Input;
		XSettingHighlights[XSettingCount]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (YSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
	{
		YSettingHighlights[YSettingCount]->SetVisibility(ESlateVisibility::Hidden);
		YSettingCount += Input;
		YSettingHighlights[YSettingCount]->SetVisibility(ESlateVisibility::Visible);
	}
}

//cross button
void USkillTreeWidget::OnDirectionalInput(int32 Input)
{
	if (CurrentSection == static_cast<int32>(ESkillTreeSection::UltimateSkill) || CurrentSection == static_cast<int32>(ESkillTreeSection::NormalSkill))
	{
		SwitchSection();
	}
	else
	{
		ChangeSkillASettingSelection(Input);
	}
}

void USkillTreeWidget::SwitchSection() // L&R Button
{
	if (CurrentSection == static_cast<int32>(ESkillTreeSection::UltimateSkill))
	{
		CurrentSection = 1;
		SkillSelectCount = 0;

		UltHighlights[UltSelectCount]->SetVisibility(ESlateVisibility::Hidden);
		UltDescriptions[UltSelectCount]->SetVisibility(ESlateVisibility::Hidden);

		SkillHighlights[0]->SetVisibility(ESlateVisibility::Visible);
		SkillDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (CurrentSection == static_cast<int32>(ESkillTreeSection::NormalSkill))
	{
		CurrentSection = 0;
		UltSelectCount = 0;

		SkillHighlights[SkillSelectCount]->SetVisibility(ESlateVisibility::Hidden);
		SkillDescriptions[SkillSelectCount]->SetVisibility(ESlateVisibility::Hidden);

		UltHighlights[0]->SetVisibility(ESlateVisibility::Visible);
		UltDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	}

}

void USkillTreeWidget::InitializeSelection() //Renaming Required 
{
	UltSelectCount = 0;
	UltHighlights[0]->SetVisibility(ESlateVisibility::Visible);
	UltDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	for (int i = 1; i < ULTNUM; i++)
	{
		UltHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
		UltDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int i = 0; i < SKILLNUM; i++)
	{
		SkillHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
		SkillDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
	}

}

// A Button
void USkillTreeWidget::SetSkillTree()
{
	if (CurrentSection == static_cast<int32>(ESkillTreeSection::UltimateSkill)) //Ult
	{
		UltActivated[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);
		UltSymbols[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);
		for (int i = 0; i < ULTNUM; i++)
		{
			if (i != UltSelectCount)
			{
				UltActivated[i]->SetVisibility(ESlateVisibility::Hidden);
				UltSymbols[i]->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		SkillTree->SetVisibility(ESlateVisibility::Visible);
		SkillTreeGrey->SetVisibility(ESlateVisibility::Hidden);

		//Broadcast Delegate to PlayerCharacter 
		OnSkillConfirmed.Broadcast(ESkillConfirmType::UltimateSkill, UltSelectCount);
	}
	else if (CurrentSection == static_cast<int32>(ESkillTreeSection::NormalSkill)) //Skill
	{
		SkillActivated[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);

		//Ult TimeUP Skill
		if (SkillSelectCount == 4)
		{
			OnSkillConfirmed.Broadcast(ESkillConfirmType::SkillTimeUp, SkillSelectCount);
		}
		else
		{
			OnSkillConfirmed.Broadcast(ESkillConfirmType::NormalSkill, SkillSelectCount);
		}
	}
	else if (CurrentSection == static_cast<int32>(ESkillTreeSection::SkillASetting)) //SkillA Setting
	{
		if (XSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
		{
			XSettingNums[XSettingCount]->SetVisibility(ESlateVisibility::Visible);

			for (int i = 0; i < XSettingNums.Num(); i++)
			{
				if (i != XSettingCount)
				{
					XSettingNums[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			OnSkillConfirmed.Broadcast(ESkillConfirmType::KeyBindingX, XSettingCount);
		}
		else if (YSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
		{
			YSettingNums[YSettingCount]->SetVisibility(ESlateVisibility::Visible);

			for (int i = 0; i < YSettingNums.Num(); i++)
			{
				if (i != YSettingCount)
				{
					YSettingNums[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			OnSkillConfirmed.Broadcast(ESkillConfirmType::KeyBindingY, YSettingCount);
		}
	}
}

void USkillTreeWidget::SetSkillPointText(int32 SkillPoint)
{
	if (SkillPointText)
	{
		SkillPointText->SetText(FText::AsNumber(SkillPoint));
	}
}

void USkillTreeWidget::InitializeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills)
{
	//Update Ult UI
	SkillTree->SetVisibility(ESlateVisibility::Visible);
	SkillTreeGrey->SetVisibility(ESlateVisibility::Hidden);
	UltActivated[UltNum]->SetVisibility(ESlateVisibility::Visible);
	UltSymbols[UltNum]->SetVisibility(ESlateVisibility::Visible);

	//Update Skill UI

	for (int i = 0; i < ActivatedSkills.Num(); i++)
	{
		if (i == 1)
		{
			SkillActivated[SkillSelectCount]->SetVisibility(ESlateVisibility::Visible);
		}
	}

}

void USkillTreeWidget::ReadyToSkillASetting(int32 Input)
{
	//Only When Ult 1 is Selected
	if (Ult1Highlight->GetVisibility() == ESlateVisibility::Visible)
	{
		if (Input == 1)
		{
			XSettingHighlight->SetVisibility(ESlateVisibility::Visible);
			YSettingHighlight->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (Input == -1)
		{
			YSettingHighlight->SetVisibility(ESlateVisibility::Visible);
			XSettingHighlight->SetVisibility(ESlateVisibility::Hidden);
		}

		CurrentSection = static_cast<int32>(ESkillTreeSection::SkillASetting);
	}
}



int32 USkillTreeWidget::GetXorYSettingCount()
{
	if (XSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
	{
		return XSettingCount;
	}

	return YSettingCount;
}

void USkillTreeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UltHighlights = { Ult1Highlight, Ult2Highlight, Ult3Highlight };
	UltActivated = { Ult1Activated, Ult2Activated, Ult3Activated };
	UltDescriptions = { Ult1Description, Ult2Description, Ult3Description };
	UltSymbols = { Ult1Symbol, Ult2Symbol, Ult3Symbol };

	SkillHighlights = { Skill1Highlight, Skill2Highlight, Skill3Highlight, Skill4Highlight, Skill5Highlight };
	SkillDescriptions = { Skill1Description, Skill2Description, Skill3Description, Skill4Description, Skill5Description };
	SkillActivated = { Skill1Activated, Skill2Activated, Skill3Activated, Skill4Activated, Skill5Activated };

	XSettingNums = { X1, X2, X3, X4 };
	YSettingNums = { Y1, Y2, Y3, Y4 };
	XSettingHighlights = { X1Highlight, X2Highlight, X3Highlight, X4Highlight };
	YSettingHighlights = { Y1Highlight, Y2Highlight, Y3Highlight, Y4Highlight };
}

