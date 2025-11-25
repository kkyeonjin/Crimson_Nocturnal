// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerComponents/HUDComponent.h"
#include "UI/MainHUDWidget.h"
#include "UI/SkillTreeWidget.h"
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
}

void UHUDComponent::HandleSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index)
{
	OnPlayerSkillConfirmed.Broadcast(ConfirmType, Index);
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
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->AddToViewport();
	}
}

void UHUDComponent::HideSkillTreeUI()
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->RemoveFromParent();
	}
}

void UHUDComponent::ChangeSelection(int32 Input)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->ChangeSelection(Input);
	}
}

void UHUDComponent::SwitchSection()
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->SwitchSection();
	}
}

void UHUDComponent::HandleSkillTreeNavigation(int32 Input)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->OnDirectionalInput(Input);
	}
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

void UHUDComponent::InitializeSkillTreeSelection()
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->InitializeSelection();
	}
}

void UHUDComponent::InitializeSkillTreeUIFromSave(int32 UltNum, TArray<int32> ActivatedSkills)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->InitializeUIFromSave(UltNum, ActivatedSkills);
	}
}

void UHUDComponent::ReadyToSkillASetting(int32 Input)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->ReadyToSkillASetting(Input);
	}
}

void UHUDComponent::ChangeSkillASettingSelection(int32 Input)
{
	if (SkillTreeUIWidget)
	{
		SkillTreeUIWidget->ChangeSkillASettingSelection(Input);
	}
}

// Called every frame
void UHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

