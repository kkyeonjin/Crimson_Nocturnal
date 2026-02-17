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
		FText HealthText = FText::Format(
			FText::FromString(TEXT("{0}/{1}")),  // ÅÛÇÃ¸´
			FText::AsNumber(Current),             // {0}
			FText::AsNumber(Max)                  // {1}
		);

		HPText->SetText(HealthText);
	}
}

void UMainHUDWidget::UpdateBP(int32 Current, int32 Max)
{
	float Percent = (float)Current / (float)Max;

	if (Percent == 1.f)
	{
		IsReadyToUseUlt = true;
	}

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
		FText BloodText = FText::Format(
			FText::FromString(TEXT("{0}/{1}")),  // ÅÛÇÃ¸´
			FText::AsNumber(Current),             // {0}
			FText::AsNumber(Max)                  // {1}
		);

		BPText->SetText(BloodText);
	}
}

void UMainHUDWidget::UpdateLevelText(int32 CurrentLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("Update Level Text"));

	if (LevelText)
	{
		FText Level = FText::Format(
			FText::FromString(TEXT("Lv. {0}")),
			FText::AsNumber(CurrentLevel)
		);

		UE_LOG(LogTemp, Warning, TEXT("Level Text: %d"), CurrentLevel);

		LevelText->SetText(Level);
	}
}

//called when game start (to initialize hud)
void UMainHUDWidget::SetUltEmblem(int32 UltNum)
{
	if (UltNum != 10) //10 is for No Equipped skill
	{
		for (int i = 0; i < UltReadyImages.Num(); i++)
		{
			UltReadyImages[i]->SetVisibility(ESlateVisibility::Hidden);
		}

		for (int i = 0; i < UltImages.Num(); i++)
		{
			UltImages[i]->SetVisibility(ESlateVisibility::Hidden);
		}

		if (IsReadyToUseUlt)
		{
			UltReadyImages[UltNum]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UltImages[UltNum]->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UMainHUDWidget::ShowReadyToUseUlt()
{
	IsReadyToUseUlt = true; 

	for (int i = 0; i < UltReadyImages.Num(); i++)
	{
		if (UltImages[i]->GetVisibility() == ESlateVisibility::Visible)
		{
			UltReadyImages[i]->SetVisibility(ESlateVisibility::Visible);
		}
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

void UMainHUDWidget::HideLevelUp()
{
	LevelUp->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDWidget::HidePopUp()
{
	if (SpecialLevelUp)
	{
		SpecialLevelUp->SetVisibility(ESlateVisibility::Hidden);
	}
	StartPopUp->SetVisibility(ESlateVisibility::Hidden);
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
	DancePopUp->SetVisibility(ESlateVisibility::Hidden);
	InteractPopUp->SetVisibility(ESlateVisibility::Hidden);
}

void UMainHUDWidget::ShowDialogue(const FString& NewDialogueName, const FString& NewDialogueLine)
{
	UE_LOG(LogTemp, Display, TEXT("ShowDialogue in MainHUDWidget"));
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

void UMainHUDWidget::ShowDancePopUp()
{
	DancePopUp->SetVisibility(ESlateVisibility::Visible);
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

	UltImages = { UltA, UltB, UltC };

	UltReadyImages = { UltAReady, UltBReady, UltCReady };

}

void UMainHUDWidget::OnSkillStarted(int32 SkillNum)
{
	SkillScreenOverlayWidget->ShowSkillEffect(SkillNum);
	UltReadyImages[SkillNum]->SetVisibility(ESlateVisibility::Hidden);
	IsReadyToUseUlt = false;
}

void UMainHUDWidget::OnSkillFinished()
{
	SkillScreenOverlayWidget->HideSkillEffect();
}

