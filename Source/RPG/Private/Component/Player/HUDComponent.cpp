#include "Component/Player/HUDComponent.h"
#include "UI/MainHUDWidget.h"
#include "UI/SkillTreeWidget.h"
#include "UI/MenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "Character/Player/PlayerCharacter.h"


UHUDComponent::UHUDComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MainHUDClass)
	{
		MainHUDWidget = CreateWidget<UMainHUDWidget>(GetWorld(), MainHUDClass);
		if (MainHUDWidget)
		{
			MainHUDWidget->AddToViewport();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MainHUDWidgetNotFound"));
		}
	}

	if (SkillTreeUIClass)
	{
		SkillTreeUIWidget = CreateWidget<USkillTreeWidget>(GetWorld(), SkillTreeUIClass);

		SkillTreeUIWidget->OnSkillConfirmed.AddDynamic(this, &UHUDComponent::HandleSkillConfirmed);
		SkillTreeUIWidget->OnSkillTreeClose.AddDynamic(this, &UHUDComponent::CloseSkillTreeUI);
	}

	if (MenuUIClass)
	{
		MenuUIWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuUIClass);
		MenuUIWidget->OnMenuClose.AddDynamic(this, &UHUDComponent::CloseMenuUI);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MenuWidgetNotFound"));
	}

	// For safe UI initializing(setting)
	OnWidgetsReady.Broadcast();
}

void UHUDComponent::HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index)
{
	OnPlayerSkillConfirmed.Broadcast(ConfirmType, Index);

	if (MainHUDWidget && ConfirmType == ESkillConfirmType::UltimateSkill)
	{
		MainHUDWidget->SetUltEmblem(Index);
	}
}

void UHUDComponent::SetupInputMode(APlayerController* PC, UUserWidget* WidgetToFocus, bool bGamePaused)
{
	if (!PC) return;

	if (bGamePaused)
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(WidgetToFocus->TakeWidget());
		PC->SetInputMode(InputMode);
	}
	else
	{
		GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
		PC->SetInputMode(FInputModeGameOnly());
	}
}

void UHUDComponent::ShowMenuUI()
{
	//Stop GamePlay 
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !MenuUIWidget) return;

	MenuUIWidget->AddToViewport(100);
	MenuUIWidget->SetKeyboardFocus();
	//Hide Main HUD UI
	MainHUDWidget->RemoveFromParent();

	SetupInputMode(PC, MenuUIWidget, /*paused=*/true);
}

void UHUDComponent::CloseMenuUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !MenuUIWidget) return;

	SetupInputMode(PC, MenuUIWidget, /*restart=*/false);

	MenuUIWidget->RemoveFromParent();
	MainHUDWidget->AddToViewport(100);
}

void UHUDComponent::UpdatePlayerHP(int32 Current, int32 Max)
{
	if (MainHUDWidget) MainHUDWidget->UpdateHP(Current, Max);
}

void UHUDComponent::UpdatePlayerBP(int32 Current, int32 Max)
{
	if (MainHUDWidget) MainHUDWidget->UpdateBP(Current, Max);
}

void UHUDComponent::UpdateLevelText(int32 CurrentLevel)
{
	if (MainHUDWidget) MainHUDWidget->UpdateLevelText(CurrentLevel);
}

void UHUDComponent::ShowSkillTreeUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !SkillTreeUIWidget) return;

	SkillTreeUIWidget->AddToViewport(100);
	SkillTreeUIWidget->SetKeyboardFocus();
	SkillTreeUIWidget->InitializeSelection();

	SetupInputMode(PC, SkillTreeUIWidget, /*paused=*/true);
}

void UHUDComponent::CloseSkillTreeUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !SkillTreeUIWidget) return;

	SetupInputMode(PC, SkillTreeUIWidget, /*restart=*/false);

	SkillTreeUIWidget->RemoveFromParent();
}

//Initialize when open new level
void UHUDComponent::SetSkillTree(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting)
{
	if (MainHUDWidget) MainHUDWidget->SetUltEmblem(UltNum);

	if (SkillTreeUIWidget) SkillTreeUIWidget->InitializeSkillTree(SkillPoint, UltNum, ActivatedSkills, Ult1XSetting, Ult1YSetting);
}

void UHUDComponent::SetSkillPointText(int32 SkillPoint)
{
	if (SkillTreeUIWidget) SkillTreeUIWidget->SetSkillPointText(SkillPoint);
}

void UHUDComponent::InitializeSkillTreeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills)
{
	if (SkillTreeUIWidget) SkillTreeUIWidget->InitializeUIFromSave(UltNum, ActivatedSkills);
}

void UHUDComponent::ShowReadyToUseUlt()
{
	if (MainHUDWidget) MainHUDWidget->ShowReadyToUseUlt();
}

void UHUDComponent::ShowLevelUpPopUp(int32 CurrentLevel)
{
	MainHUDWidget->ShowLevelUpPopUp(CurrentLevel);

	GetWorld()->GetTimerManager().SetTimer(
		LevelUpPopUpTimer,
		this,
		&UHUDComponent::HideLevelUp,
		2.5f,
		false
	); 
}

void UHUDComponent::HideLevelUp()
{
	if (MainHUDWidget) MainHUDWidget->HideLevelUp();
}

void UHUDComponent::HidePopUp()
{
	if (MainHUDWidget) MainHUDWidget->HidePopUp();
}

void UHUDComponent::ShowStartPopUp()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	PC->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	MainHUDWidget->ShowStartPopUp();
}

void UHUDComponent::ShowComboPopUp(int32 ComboNum)
{
	if (MainHUDWidget) MainHUDWidget->ShowComboPopUp(ComboNum);

	GetWorld()->GetTimerManager().SetTimer(
		ComboPopUpTimer,
		this,
		&UHUDComponent::HideComboPopUp,
		1.5f,
		false
	); 
}

void UHUDComponent::HideComboPopUp()
{
	if (MainHUDWidget) MainHUDWidget->HideComboPopUp();
}

void UHUDComponent::ShowSkillEffect(int32 SkillNum)
{
	if (MainHUDWidget) MainHUDWidget->OnSkillStarted(SkillNum);
}

void UHUDComponent::HideSkillEffect()
{
	if (MainHUDWidget) MainHUDWidget->OnSkillFinished();
}

void UHUDComponent::ShowInteractPopUp()
{
	if (MainHUDWidget) MainHUDWidget->ShowInteractPopUp();
}

void UHUDComponent::HideInteractPopUp()
{
	if (MainHUDWidget) MainHUDWidget->HideInteractPopUp();
}

void UHUDComponent::ShowDancePopUp()
{
	if (MainHUDWidget) MainHUDWidget->ShowDancePopUp();
}

void UHUDComponent::ShowDialogue(const FString& DialogueName, const FString& DialogueLine)
{
	if (MainHUDWidget) MainHUDWidget->ShowDialogue(DialogueName, DialogueLine);
}

void UHUDComponent::HideDialogue()
{
	if (MainHUDWidget) MainHUDWidget->HideDialogue();
}

void UHUDComponent::ShowQTEUI(int32 ComboNum)
{
	if (MainHUDWidget) MainHUDWidget->ShowQTEUI(ComboNum);
}

void UHUDComponent::HideQTEUI(int32 ComboNum)
{
	if (MainHUDWidget) MainHUDWidget->HideQTEUI(ComboNum);
}

void UHUDComponent::NoticeToBreak()
{
	if (MainHUDWidget) MainHUDWidget->NoticeToBreak();
}



