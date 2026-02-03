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
	}

	if (MenuUIClass)
	{
		MenuUIWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuUIClass);
	}
}

void UHUDComponent::HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index)
{
	OnPlayerSkillConfirmed.Broadcast(ConfirmType, Index);
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
}

void UHUDComponent::CloseMenuUI()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !MenuUIWidget) return;

	GetWorld()->GetWorldSettings()->SetTimeDilation(1.0f);
	PC->SetInputMode(FInputModeGameOnly());
	MenuUIWidget->RemoveFromParent();
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

void UHUDComponent::ShowSkillTreeUI()
{
	//Stop GamePlay 
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC || !SkillTreeUIWidget) return;

	GetWorld()->GetWorldSettings()->SetTimeDilation(0.0f);

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

void UHUDComponent::SetSkillTree()
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->SetSkillTree();
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

void UHUDComponent::ShowLevelUpPopUp(int32 CurrentLevel)
{
	MainHUDWidget->ShowLevelUpPopUp(CurrentLevel);

	//ComboTimerOutTimer Starts
	GetWorld()->GetTimerManager().SetTimer(
		LevelUpPopUpTimer,
		this,
		&UHUDComponent::HideLevelUpPopUp,
		3.f,
		false
	); //last parameter false-> call Timer just once 

}

void UHUDComponent::HideLevelUpPopUp()
{
	MainHUDWidget->HideLevelUpPopUp();
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

	//ComboTimerOutTimer Starts
	GetWorld()->GetTimerManager().SetTimer(
		SkillEffectTimer,
		this,
		&UHUDComponent::HideSkillEffect,
		4.f,
		false
	); //last parameter false-> call Timer just once 

	UE_LOG(LogTemp, Display, TEXT("HUDCOMP->SkillEffect"));
	UE_LOG(LogTemp, Display, TEXT("SkillNum: %d"), SkillNum);
	
}

void UHUDComponent::HideSkillEffect()
{
	MainHUDWidget->OnSkillFinished();
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



