// Screen Effect appears when player's Ultimate Skill is activated

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "SkillScreenOverlayWidget.generated.h"

UCLASS()
class RPG_API USkillScreenOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void ShowSkillEffect(int32 SkillNum);

	UFUNCTION()
	void HideSkillEffect();

	UPROPERTY(meta = (BindWidget))
	UImage* BloodRush;

	UPROPERTY(meta = (BindWidget))
	UImage* HPDrain;

	UPROPERTY(meta = (BindWidget))
	UImage* Infect;

	UPROPERTY(EditAnywhere)
	UWidgetAnimation* BloodRushAnim;

private:
	TArray<UImage*> SkillImages;

};
