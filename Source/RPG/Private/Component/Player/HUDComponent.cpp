// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/HUDComponent.h"
#include "UI/MainHUDWidget.h"
#include "UI/SkillTreeWidget.h"
#include "UI/MenuWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UHUDComponent::UHUDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
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

		UE_LOG(LogTemp, Warning, TEXT("MenuWidgetFound"));

		MenuUIWidget->OnMenuClose.AddDynamic(this, &UHUDComponent::CloseMenuUI);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MenuWidgetNotFound"));
	}
}

void UHUDComponent::HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index)
{
	OnPlayerSkillConfirmed.Broadcast(ConfirmType, Index);

	if (MainHUDWidget && ConfirmType == ESkillConfirmType::UltimateSkill)
	{
		MainHUDWidget->SetUltEmblem(Index);
	}
}

void UHUDComponent::ShowMenuUI()
{
	//Stop GamePlay 
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !MenuUIWidget) return;

	GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MenuUIWidget->TakeWidget());
	PC->SetInputMode(InputMode);

	MenuUIWidget->AddToViewport(100);
	MenuUIWidget->SetKeyboardFocus();

	//Hide Main HUD UI
	MainHUDWidget->RemoveFromParent();
}

void UHUDComponent::CloseMenuUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !MenuUIWidget) return;

	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	PC->SetInputMode(FInputModeGameOnly());
	MenuUIWidget->RemoveFromParent();

	MainHUDWidget->AddToViewport(100);
}

void UHUDComponent::UpdatePlayerHP(int32 Current, int32 Max)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateHP(Current, Max);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainHUDWidgetNotFound"));
	}
}

void UHUDComponent::UpdatePlayerBP(int32 Current, int32 Max)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->UpdateBP(Current, Max);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainHUDWidgetNotFound"));
	}
}

void UHUDComponent::UpdateLevelText(int32 CurrentLevel)
{
	if (MainHUDWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateLevelTextIn HUDComp"));
		MainHUDWidget->UpdateLevelText(CurrentLevel);
	}
}

void UHUDComponent::ShowSkillTreeUI()
{
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !SkillTreeUIWidget) return;

	//Stop GamePlay 
	GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);

	//Change InputMode to UI InputMode
	//for Use of BaseUIInputWidget::NativeOnKeyDown 
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(SkillTreeUIWidget->TakeWidget());
	PC->SetInputMode(InputMode);

	SkillTreeUIWidget->AddToViewport(100);

	SkillTreeUIWidget->SetKeyboardFocus();

	SkillTreeUIWidget->InitializeSelection();
}

void UHUDComponent::CloseSkillTreeUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !SkillTreeUIWidget) return;

	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	PC->SetInputMode(FInputModeGameOnly());
	SkillTreeUIWidget->RemoveFromParent();
}

//Initialize when open new level
void UHUDComponent::SetSkillTree(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting)
{
	UE_LOG(LogTemp, Error, TEXT("Initial Setting for SkillTree,,, in HUDComp"));

	if (MainHUDWidget)
	{
		MainHUDWidget->SetUltEmblem(UltNum);
	}
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->InitializeSkillTree(SkillPoint, UltNum, ActivatedSkills, Ult1XSetting, Ult1YSetting);
	}
}

void UHUDComponent::SetSkillPointText(int32 SkillPoint)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->SetSkillPointText(SkillPoint);
	}
}

void UHUDComponent::InitializeSkillTreeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->InitializeUIFromSave(UltNum, ActivatedSkills);
	}
}

void UHUDComponent::ShowReadyToUseUlt()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->ShowReadyToUseUlt();
	}
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
	); //last parameter false-> call Timer just once 
}

void UHUDComponent::HideLevelUp()
{
	MainHUDWidget->HideLevelUp();
}

void UHUDComponent::HidePopUp()
{
	MainHUDWidget->HidePopUp();
}

void UHUDComponent::ShowStartPopUp()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Error, TEXT("SetInputModeToGAMEONLY"));
	PC->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	MainHUDWidget->ShowStartPopUp();
}

void UHUDComponent::ShowComboPopUp(int32 ComboNum)
{
	MainHUDWidget->ShowComboPopUp(ComboNum);

	//ComboTimerOutTimer Starts
	GetWorld()->GetTimerManager().SetTimer(
		ComboPopUpTimer,
		this,
		&UHUDComponent::HideComboPopUp,
		1.5f,
		false
	); //last parameter false-> call Timer just once 

}

void UHUDComponent::HideComboPopUp()
{
	MainHUDWidget->HideComboPopUp();
}

void UHUDComponent::ShowSkillEffect(int32 SkillNum)
{
	MainHUDWidget->OnSkillStarted(SkillNum);
}

void UHUDComponent::HideSkillEffect()
{
	MainHUDWidget->OnSkillFinished();
}

void UHUDComponent::ShowInteractPopUp()
{
	MainHUDWidget->ShowInteractPopUp();
}

void UHUDComponent::HideInteractPopUp()
{
	MainHUDWidget->HideInteractPopUp();
}

void UHUDComponent::ShowDancePopUp()
{
	MainHUDWidget->ShowDancePopUp();
}

void UHUDComponent::ShowDialogue(const FString& DialogueName, const FString& DialogueLine)
{
	MainHUDWidget->ShowDialogue(DialogueName, DialogueLine);
}

void UHUDComponent::HideDialogue()
{
	MainHUDWidget->HideDialogue();
}

void UHUDComponent::ShowQTEUI(int32 ComboNum)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->ShowQTEUI(ComboNum);
	}
}

void UHUDComponent::HideQTEUI(int32 ComboNum)
{
	if (MainHUDWidget)
	{
		MainHUDWidget->HideQTEUI(ComboNum);
	}
}

void UHUDComponent::NoticeToBreak()
{
	if (MainHUDWidget)
	{
		MainHUDWidget->NoticeToBreak();
	}
}



