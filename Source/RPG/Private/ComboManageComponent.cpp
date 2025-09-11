// Fill out your copyright notice in the Description page of Project Settings.
#include "ComboManageComponent.h"


// Sets default values for this component's properties
UComboManageComponent::UComboManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InitializeComboPattern();

	// ...
}


// Called when the game starts
void UComboManageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UComboManageComponent::ResetInputSequence()
{
	InputKeySequence.Empty();
	TimeSinceLastInput = 0.f;
}

void UComboManageComponent::AddInputKey(EAttackKeyType Key)
{
	InputKeySequence.Add(Key);
	TimeSinceLastInput = 0.f;

	UE_LOG(LogTemp, Warning, TEXT("InputKeySequence"));

	for (int32 i = 0; i < ComboPatterns.Num(); i++)
	{
		//Compare InputSequence to Designated Combo Patterns
		if (InputKeySequence.Num() == ComboPatterns[i].ComboKeySequence.Num())
		{
			bool IsComboMatch = true;
			for (int32 j = 0; j < InputKeySequence.Num(); j++)
			{
				if (InputKeySequence[j] != ComboPatterns[i].ComboKeySequence[j])
				{
					IsComboMatch = false;
					break;
				}
			}
			if (IsComboMatch)
			{
				//start broadcast when combo matches
				OnComboComplete.Broadcast(ComboPatterns[i].ComboNum);
				UE_LOG(LogTemp, Warning, TEXT("ComboMatch!!"));
				ResetInputSequence();
				break;
			}
		}

	}

}

void UComboManageComponent::InitializeComboPattern()
{
	//ComboA
	FComboPattern ComboA;
	ComboA.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::X, EAttackKeyType::Y };
	ComboA.ComboNum = 0;
	ComboPatterns.Add(ComboA);

	//ComboB
	FComboPattern ComboB;
	ComboB.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::X };
	ComboB.ComboNum = 1;
	ComboPatterns.Add(ComboB);
}

void UComboManageComponent::CheckComboTimeOut(float DeltaTime)
{
	TimeSinceLastInput += DeltaTime;

	if (TimeSinceLastInput > ComboTimeOut)
	{
		ResetInputSequence();
		UE_LOG(LogTemp, Warning, TEXT("InputReset"));
	}
}

UAnimMontage* UComboManageComponent::GetComboAnimMontage(int ComboNum)
{
	if (!ComboPatterns.IsValidIndex(ComboNum))
	{
		return nullptr;
	}

	if (ComboPatterns[ComboNum].ComboAnimMontage.IsValid())
	{
		return ComboPatterns[ComboNum].ComboAnimMontage.Get();
	}
	
	return ComboPatterns[ComboNum].ComboAnimMontage.LoadSynchronous();

}

// Called every frame
void UComboManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckComboTimeOut(DeltaTime);
	// ...
}