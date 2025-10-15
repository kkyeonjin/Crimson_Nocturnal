// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/PlayerComponents/ComboManageComponent.h"


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

//bool값 반환하도록 (IsComboMatch뭐 이런 이름으로 바꾸고)
bool UComboManageComponent::IsComboMatch(EAttackKeyType Key) //이름다시지을필요가존나있음 
{
	InputKeySequence.Add(Key);
	TimeSinceLastInput = 0.f;

	GetWorld()->GetTimerManager().SetTimer(
		ComboTimeOutTimer, this, &UComboManageComponent::CheckComboTimeOut, ComboTimeOut, false
	); //last parameter false-> call Timer just once 

	for (int32 i = 0; i < ComboPatterns.Num(); i++)
	{
		//Compare InputSequence to Designated Combo Patterns
		if (InputKeySequence.Num() == ComboPatterns[i].ComboKeySequence.Num())
		{
			bool bIsComboMatch = true;
			for (int32 j = 0; j < InputKeySequence.Num(); j++)
			{
				if (InputKeySequence[j] != ComboPatterns[i].ComboKeySequence[j])
				{
					bIsComboMatch = false;
					return bIsComboMatch;
				}
			}
			if (bIsComboMatch)
			{
				//Set AttackMultiplier
				CurrentAttackMultiplier = ComboPatterns[i].ComboAttackMultiplier;
				//start broadcast when combo matches
				OnComboMatch.Broadcast(ComboPatterns[i].AnimMontageNum, ComboPatterns[i].BPValue);
				UE_LOG(LogTemp, Warning, TEXT("ComboMatch!!"));
				ResetInputSequence();
				return bIsComboMatch;
			}
		}
	}
	return false;
}

void UComboManageComponent::ChainAttackCount(EAttackKeyType Key)
{
	FString SectionNameStr;
	int32 AnimNum;

	if (Key == EAttackKeyType::X)
	{
		if (ChainAttackCountX == 3)
		{
			ChainAttackCountX = 0;
		}
		ChainAttackCountX++;
		UE_LOG(LogTemp, Display, TEXT("CountX : %d"), ChainAttackCountX);
		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountX);
		AnimNum = 0;

		//Set CurrentAttackMultiplier for each ChainCountSteps
		switch (ChainAttackCountX)
		{
		case 1: CurrentAttackMultiplier = 1.0f; break;
		case 2: CurrentAttackMultiplier = 1.2f; break;
		case 3: CurrentAttackMultiplier = 1.5f; break;
		default: CurrentAttackMultiplier = 1.0f; break;
		}
	}
	else if (Key == EAttackKeyType::Y)
	{
		//Set CurrentAttackMultiplier for each ChainCountSteps
		if (ChainAttackCountY == 2)
		{
			ChainAttackCountY = 0;
		}
		ChainAttackCountY++;
		UE_LOG(LogTemp, Display, TEXT("CountY : %d"), ChainAttackCountX);
		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountY);
		AnimNum = 1;

		switch (ChainAttackCountY)
		{
		case 1: CurrentAttackMultiplier = 1.0f; break;
		case 2: CurrentAttackMultiplier = 1.3f; break;
		default: CurrentAttackMultiplier = 1.0f; break;
		}
	}
	FName SectionName(*SectionNameStr);
	OnChainAttack.Broadcast(AnimNum, SectionName);
}

void UComboManageComponent::InitializeComboPattern()
{
	//ComboA
	FComboPattern ComboA;
	ComboA.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::X, EAttackKeyType::Y };
	ComboA.ComboNum = 0;
	ComboA.AnimMontageNum = 10;
	ComboA.ComboAttackMultiplier = 3.5;
	ComboA.BPValue = 3;
	ComboPatterns.Add(ComboA);

	//ComboB
	FComboPattern ComboB;
	ComboB.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::X };
	ComboB.ComboNum = 1;
	ComboB.AnimMontageNum = 11;
	ComboB.ComboAttackMultiplier = 2.8;
	ComboB.BPValue = 2;
	ComboPatterns.Add(ComboB);
}

void UComboManageComponent::CheckComboTimeOut()
{
	ResetInputSequence();
	ChainAttackCountX = 0;
	ChainAttackCountY = 0;
	UE_LOG(LogTemp, Warning, TEXT("InputReset"));
}

int32 UComboManageComponent::GetComboAnimMontage(int32 ComboAnimNum)
{
	return ComboPatterns[ComboAnimNum].AnimMontageNum;
}



// Called every frame
void UComboManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CheckComboTimeOut(DeltaTime);
	// ...
}