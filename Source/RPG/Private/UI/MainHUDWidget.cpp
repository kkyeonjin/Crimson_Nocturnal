// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUDWidget.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/SkillScreenOverlayWidget.h"


void UMainHUDWidget::UpdateHP(int32 Current, int32 Max)
{
	float Percent = (float)Current / (float)Max;
	//UE_LOG(LogTemp, Error, TEXT("HP Percent: %f"), Percent);

	if (HPBar)
	{
		HPBar->SetPercent(Percent);
		UE_LOG(LogTemp, Error, TEXT("HPBarUpdate"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HPBarCANNOTFound"));
	}

	if (HPText)
	{
		HPText->SetText(FText::AsNumber(Current));
	}
}

void UMainHUDWidget::UpdateBP(int32 Current, int32 Max)
{
	float Percent = (float)Current / (float)Max;
	//UE_LOG(LogTemp, Error, TEXT("HP Percent: %f"), Percent);

	if (BPBar)
	{
		BPBar->SetPercent(Percent);
		UE_LOG(LogTemp, Error, TEXT("BPBarUpdate"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BPBarCANNOTFound"));
	}

	if (BPText)
	{
		BPText->SetText(FText::AsNumber(Current));
	}
}

void UMainHUDWidget::ShowQTEUI(int32 ComboNum)
{
	if (ComboQTEImages.Num() != 0)
	{
		ComboQTEImages[ComboNum]->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainHUDWidget::HideQTEUI(int32 ComboNum)
{
	if (ComboQTEImages.Num() != 0)
	{
		ComboQTEImages[ComboNum]->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUDWidget::ShowLevelUpPopUp(int32 CurrentLevel)
{
	if (CurrentLevel == 2)
	{
		Level2PopUp->SetVisibility(ESlateVisibility::Visible);
		SpecialLevelUp = Level2PopUp;
	}
	else if (CurrentLevel == 5)
	{
		Level5PopUp->SetVisibility(ESlateVisibility::Visible);
		SpecialLevelUp = Level5PopUp;
	}

	LevelUp->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUDWidget::HidePopUp()
{
	SpecialLevelUp->SetVisibility(ESlateVisibility::Hidden);
	LevelUp->SetVisibility(ESlateVisibility::Hidden);

	StartPopUp->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUDWidget::ShowStartPopUp()
{
	StartPopUp->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUDWidget::ShowComboPopUp(int32 ComboNum)
{
	ComboPopUpImages[ComboNum]->SetVisibility(ESlateVisibility::Visible);
	
}

void UMainHUDWidget::HideComboPopUp()
{
	for (int i = 0; i < ComboPopUpImages.Num(); i++)
	{
		if (ComboPopUpImages[i]->GetVisibility() == ESlateVisibility::Visible)
		{
			ComboPopUpImages[i]->SetVisibility(ESlateVisibility::Hidden);
			break;
		}
	}
}

void UMainHUDWidget::NoticeToBreak()
{
	BreakTheCrystal->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUDWidget::ShowInteractPopUp()
{
	InteractPopUp->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUDWidget::HideInteractPopUp()
{
	InteractPopUp->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDWidget::ShowDialogue(const FString& NewDialogueName, const FString& NewDialogueLine)
{
	DialogueName->SetText(FText::FromString(NewDialogueName));
	DialogueLine->SetText(FText::FromString(NewDialogueLine));

	DialoguePanel->SetVisibility(ESlateVisibility::Visible);
}

void UMainHUDWidget::HideDialogue()
{
	if (DialoguePanel->GetVisibility() == ESlateVisibility::Visible)
	{
		DialoguePanel->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UMainHUDWidget::SetSubWidgets(USkillScreenOverlayWidget* Widget)
{
	SkillScreenOverlayWidget = Widget;
}

void UMainHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ComboQTEImages = { ComboA, ComboB, ComboC, ComboD };

	ComboPopUpImages = { ComboA, ComboB, ComboC, ComboD };

}

void UMainHUDWidget::OnSkillStarted(int32 SkillNum)
{
	SkillScreenOverlayWidget->ShowSkillEffect(SkillNum);

	UE_LOG(LogTemp, Error, TEXT("Is Valid: %s"), SkillScreenOverlayWidget ? TEXT("Yes") : TEXT("No"));
}

void UMainHUDWidget::OnSkillFinished()
{
	SkillScreenOverlayWidget->HideSkillEffect();
}

