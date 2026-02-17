#include "UI/MenuWidget.h"
#include "Components/Image.h"
#include "Game/RPGGameInstance.h"
#include "UI/ControllerMapWidget.h"
#include "UI/TutorialWidget.h"


void UMenuWidget::SetSubWidget(UTutorialWidget* TutoWidget, UControllerMapWidget* ContWidget)
{
	TutorialWidget = TutoWidget;
	ControllerMapWidget = ContWidget;
}

// up&down
void UMenuWidget::OnDirectionalInput(int32 Input)
{
	MenuHighlights[CurrentMenuCount]->SetVisibility(ESlateVisibility::Hidden);

	CurrentMenuCount -= Input;
	if (CurrentMenuCount == -1)
	{
		CurrentMenuCount = 3;
	}
	else if (CurrentMenuCount == 4)
	{
		CurrentMenuCount = 0;
	}

	MenuHighlights[CurrentMenuCount]->SetVisibility(ESlateVisibility::Visible);
}

void UMenuWidget::OnSelect()
{
	switch (CurrentMenuCount)
	{
		case 0: //resume
			CloseMenu();
			break;
		case 1: //ContMap
			ControllerMapWidget->InitializeContMap();
			break;
		case 2: //Tutorial
			TutorialWidget->InitializeTutorial();
			break;
		case 3: //Exit
			if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetGameInstance()))
			{
				GI->Shutdown();  
			}
			break;
		default:
		 break;

	}
}

//Press B Button
void UMenuWidget::CloseMenu()
{
	if (ControllerMapWidget->GetVisibility() == ESlateVisibility::Hidden && TutorialWidget->GetVisibility() == ESlateVisibility::Hidden)
	{
		//Send to HUDComp in order to set MainHUDWidget as visible
		OnMenuClose.Broadcast();
	}
}

void UMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MenuHighlights = { ResumeHighlight, ContMapHighlight, TutorialHighlight, ExitHighlight };
}
