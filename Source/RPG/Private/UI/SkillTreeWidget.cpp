#include "UI/SkillTreeWidget.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

void USkillTreeWidget::ChangeSelection(int32 Input) //Up&Down Button
{
	if (CurrentSection == static_cast<int32>(ESkillTreeSection::UltimateSkill))
	{
		UltSelectCount -= Input;
		// Circular selection enable
		if (UltSelectCount == -1)
		{
			UltSelectCount = 2;
		}
		else if (UltSelectCount == ULTNUM)
		{
			UltSelectCount = 0;
		}

		UltPanels[UltSelectCount]->SetVisibility(ESlateVisibility::Visible);

		for (int i = 0; i < ULTNUM; i++)
		{
			if (i != UltSelectCount)
			{
				UltPanels[i]->SetVisibility(ESlateVisibility::Hidden);
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

void USkillTreeWidget::OnCloseSkillTree()
{
	OnSkillTreeClose.Broadcast();
}

void USkillTreeWidget::SwitchSection() // L&R Button
{
	if (CurrentSection == static_cast<int32>(ESkillTreeSection::UltimateSkill))
	{
		CurrentSection = 1;
		SkillSelectCount = 0;

		UltPanels[UltSelectCount]->SetVisibility(ESlateVisibility::Hidden);

		SkillHighlights[0]->SetVisibility(ESlateVisibility::Visible);
		SkillDescriptions[0]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (CurrentSection == static_cast<int32>(ESkillTreeSection::NormalSkill))
	{
		CurrentSection = 0;
		UltSelectCount = 0;

		SkillHighlights[SkillSelectCount]->SetVisibility(ESlateVisibility::Hidden);
		SkillDescriptions[SkillSelectCount]->SetVisibility(ESlateVisibility::Hidden);

		UltPanels[0]->SetVisibility(ESlateVisibility::Visible);
	}
}

//L&R Button when SkillA Key Setting
void USkillTreeWidget::ChangeSkillASettingSelection(int32 Input)
{
	UE_LOG(LogTemp, Display, TEXT("Current Section: %d"), CurrentSection);
	UE_LOG(LogTemp, Warning, TEXT("Current Input: %d"), Input);

	//X Setting
	if (XSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
	{
		XSettingHighlights[XSettingCount]->SetVisibility(ESlateVisibility::Hidden);
		XSettingCount += Input;

		//enable circular selection when count is over option's number
		if (XSettingCount == -1)
		{
			XSettingCount = 3;
		}
		else if (XSettingCount == 4)
		{
			XSettingCount = 0;
		}

		XSettingHighlights[XSettingCount]->SetVisibility(ESlateVisibility::Visible);
	}
	else if (YSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
	{
		YSettingHighlights[YSettingCount]->SetVisibility(ESlateVisibility::Hidden);
		YSettingCount += Input;

		//enable circular selection when count is over option's number
		if (YSettingCount == -1)
		{
			YSettingCount = 3;
		}
		else if (YSettingCount == 4)
		{
			YSettingCount = 0;
		}

		YSettingHighlights[YSettingCount]->SetVisibility(ESlateVisibility::Visible);
	}
}

//Called When SkillTree is Open (when is visible)
void USkillTreeWidget::InitializeSelection() //Renaming Required 
{
	UltSelectCount = 0;

	UltPanels[0]->SetVisibility(ESlateVisibility::Visible);
	for (int i = 1; i < ULTNUM; i++)
	{
		UltPanels[i]->SetVisibility(ESlateVisibility::Hidden);

	}

	for (int i = 0; i < SKILLNUM; i++)
	{
		SkillHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
		SkillDescriptions[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int i = 0; i < 4; i++)
	{
		XSettingHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
		YSettingHighlights[i]->SetVisibility(ESlateVisibility::Hidden);
	}

	XSettingHighlight->SetVisibility(ESlateVisibility::Hidden);
	YSettingHighlight->SetVisibility(ESlateVisibility::Hidden);

}

//Initialize SkillTree by loading save game data
void USkillTreeWidget::InitializeSkillTree(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills,
	int32 Ult1XSetting, int32 Ult1YSetting)
{
	SetSkillPointText(SkillPoint);

	SetImageArrays();

	/*
	//FIX: 빈 배열 체크
	if (ActivatedSkills.Num() == 0)
	{
		ActivatedSkills.Init(0, 5);  // 기본 5개 슬롯 0으로 채움
		UE_LOG(LogTemp, Warning, TEXT("SkillTree: ActivatedSkills was empty, initialized"));
	}

	UE_LOG(LogTemp, Warning, TEXT("XSettingNums SIZE: %d"), XSettingNums.Num());
	*/

	if (UltNum != 10) //10 is for No Equipped skill
	{
		UltActivated[UltNum]->SetVisibility(ESlateVisibility::Visible);
		UltSymbols[UltNum]->SetVisibility(ESlateVisibility::Visible);

		SkillTree->SetVisibility(ESlateVisibility::Visible);
		SkillTreeGrey->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int i = 0; i < SKILLNUM; ++i)
	{
		if (ActivatedSkills[i] == 1)
		{
			SkillActivated[i]->SetVisibility(ESlateVisibility::Visible);
		}
	}

	XSettingNums[Ult1XSetting]->SetVisibility(ESlateVisibility::Visible);
	YSettingNums[Ult1YSetting]->SetVisibility(ESlateVisibility::Visible);


	// 안전 루프: 실제 크기까지만
	int32 NumSkills = FMath::Min(ActivatedSkills.Num(), SKILLNUM);
	for (int i = 0; i < NumSkills; ++i)
	{
		if (ActivatedSkills[i] == 1)
		{
			SkillActivated[i]->SetVisibility(ESlateVisibility::Visible);
		}
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
		if (CurrentSkillPoint != 0)
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
	}
	else if (CurrentSection == static_cast<int32>(ESkillTreeSection::SkillASetting)) //SkillA Setting
	{
		if (XSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
		{
			XSettingNums[XSettingCount]->SetVisibility(ESlateVisibility::Visible);
			XSettingHighlights[XSettingCount]->SetVisibility(ESlateVisibility::Hidden);

			for (int i = 0; i < XSettingNums.Num(); i++)
			{
				if (i != XSettingCount)
				{
					XSettingNums[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			XSettingHighlight->SetVisibility(ESlateVisibility::Hidden);

			OnSkillConfirmed.Broadcast(ESkillConfirmType::KeyBindingX, XSettingCount);
		}
		else if (YSettingHighlight->GetVisibility() == ESlateVisibility::Visible)
		{
			YSettingNums[YSettingCount]->SetVisibility(ESlateVisibility::Visible);
			YSettingHighlights[YSettingCount]->SetVisibility(ESlateVisibility::Hidden);

			for (int i = 0; i < YSettingNums.Num(); i++)
			{
				if (i != YSettingCount)
				{
					YSettingNums[i]->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			YSettingHighlight->SetVisibility(ESlateVisibility::Hidden);

			OnSkillConfirmed.Broadcast(ESkillConfirmType::KeyBindingY, YSettingCount);
		}

		//back to Ult 1's Main Selection Screen
		CurrentSection = static_cast<int32>(ESkillTreeSection::UltimateSkill);

	}
}

void USkillTreeWidget::SetSkillPointText(int32 SkillPoint)
{
	if (SkillPointText)
	{
		SkillPointText->SetText(FText::AsNumber(SkillPoint));
		CurrentSkillPoint = SkillPoint;
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

// X / Y Button
void USkillTreeWidget::ReadyToSkillASetting(int32 Input)
{
	//Only When Ult 1 is Selected
	if (Ult1Highlight->GetVisibility() == ESlateVisibility::Visible)
	{
		if (Input == 1) //X button
		{
			XSettingHighlight->SetVisibility(ESlateVisibility::Visible);
			XSettingHighlights[XSettingCount]->SetVisibility(ESlateVisibility::Visible);
			YSettingHighlight->SetVisibility(ESlateVisibility::Hidden);
		}
		else if (Input == -1) //Y button
		{
			YSettingHighlight->SetVisibility(ESlateVisibility::Visible);
			YSettingHighlights[YSettingCount]->SetVisibility(ESlateVisibility::Visible);
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

	SetImageArrays();
	
	InitializeSelection();
}

void USkillTreeWidget::SetImageArrays()
{
	UltHighlights = { Ult1Highlight, Ult2Highlight, Ult3Highlight };
	UltActivated = { Ult1Activated, Ult2Activated, Ult3Activated };
	UltDescriptions = { Ult1Description, Ult2Description, Ult3Description };
	UltSymbols = { Ult1Symbol, Ult2Symbol, Ult3Symbol };
	UltPanels = { Ult1Panel, Ult2Panel, Ult3Panel };

	SkillHighlights = { Skill1Highlight, Skill2Highlight, Skill3Highlight, Skill4Highlight, Skill5Highlight };
	SkillDescriptions = { Skill1Description, Skill2Description, Skill3Description, Skill4Description, Skill5Description };
	SkillActivated = { Skill1Activated, Skill2Activated, Skill3Activated, Skill4Activated, Skill5Activated };

	XSettingNums = { X1, X2, X3, X4 };
	YSettingNums = { Y1, Y2, Y3, Y4 };
	XSettingHighlights = { X1Highlight, X2Highlight, X3Highlight, X4Highlight };
	YSettingHighlights = { Y1Highlight, Y2Highlight, Y3Highlight, Y4Highlight };

}



