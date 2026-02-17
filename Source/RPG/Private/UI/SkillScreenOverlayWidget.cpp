#include "UI/SkillScreenOverlayWidget.h"
#include "Components/Image.h"


void USkillScreenOverlayWidget::ShowSkillEffect(int32 SkillNum)
{
	NativeConstruct();

	SetVisibility(ESlateVisibility::Visible);
	SkillImages[SkillNum]->SetVisibility(ESlateVisibility::Visible);
	
}

void USkillScreenOverlayWidget::HideSkillEffect()
{
	SetVisibility(ESlateVisibility::Hidden);
}


void USkillScreenOverlayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SkillImages = { BloodRush, HPDrain, Infect };
}
