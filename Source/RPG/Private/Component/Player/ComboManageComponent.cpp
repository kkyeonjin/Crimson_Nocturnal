// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/ComboManageComponent.h"


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

//called when combo timeout is over 
void UComboManageComponent::ResetInputSequence()
{
	InputKeySequence.Empty();
	TimeSinceLastInput = 0.f;
}

//parameter: input key value from player (x/y)
//return value: whether combo matched or not (bool)
bool UComboManageComponent::IsComboMatch(EAttackKeyType Key)  
{
	//add input key in key sequence array & initialize combo time 
	InputKeySequence.Add(Key);
	TimeSinceLastInput = 0.f;

	//ComboTimerOutTimer Starts
	GetWorld()->GetTimerManager().SetTimer(
		ComboTimeOutTimer,
		this, 
		&UComboManageComponent::CheckComboTimeOut, 
		ComboTimeOut,
		false
	); //last parameter false-> call Timer just once 


	for (int32 i = 0; i < ComboPatterns.Num(); i++)
	{
		//Compare key sequence Array with Designated Combo Patterns
		if (InputKeySequence.Num() == ComboPatterns[i].ComboKeySequence.Num())
		{
			bool bIsComboMatch = true;

			for (int32 j = 0; j < InputKeySequence.Num(); j++)
			{
				if (InputKeySequence[j] != ComboPatterns[i].ComboKeySequence[j])
				{
					bIsComboMatch = false;
					break;
				}
			}
			if (bIsComboMatch)
			{
				//Set AttackMultiplier
				CurrentAttackMultiplier = ComboPatterns[i].ComboAttackMultiplier;

				//start broadcast when combo matches
				OnComboMatch.Broadcast(ComboPatterns[i].ComboNum, ComboPatterns[i].BPValue);
				UE_LOG(LogTemp, Warning, TEXT("ComboMatch!!"));

				//reset key sequence array for new combo
				ResetInputSequence();
				return bIsComboMatch;
			}
		}
	}
	return false;
}

//parameter: input key value from player (x/y)
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
		//UE_LOG(LogTemp, Display, TEXT("CountX : %d"), ChainAttackCountX);
		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountX);
		AnimNum = 0;

		//Set CurrentAttackMultiplier for each ChainCountSteps
		switch (ChainAttackCountX)
		{
		case 1: CurrentAttackMultiplier = ComboConstants::CHAIN_X1_MULTIPLIER; break;
		case 2: CurrentAttackMultiplier = ComboConstants::CHAIN_X2_MULTIPLIER; break;
		case 3: CurrentAttackMultiplier = ComboConstants::CHAIN_X3_MULTIPLIER; break;
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
		//UE_LOG(LogTemp, Display, TEXT("CountY : %d"), ChainAttackCountX);
		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountY);
		AnimNum = 1;

		switch (ChainAttackCountY)
		{
		case 1: CurrentAttackMultiplier = ComboConstants::CHAIN_Y1_MULTIPLIER; break;
		case 2: CurrentAttackMultiplier = ComboConstants::CHAIN_Y2_MULTIPLIER; break;
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
	ComboA.ComboType = EComboType::PowerStrike;
	ComboA.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::X, EAttackKeyType::Y };
	ComboA.ComboNum = 0;
	ComboA.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboA);
	ComboA.ComboAttackMultiplier = 3.5;
	ComboA.BPValue = 3;
	ComboPatterns.Add(ComboA);

	//ComboB
	FComboPattern ComboB;
	ComboB.ComboType = EComboType::AdvancedCombo;
	ComboB.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::X };
	ComboB.ComboNum = 1;
	ComboB.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboB);
	ComboB.ComboAttackMultiplier = 2.8;
	ComboB.BPValue = 2;
	ComboPatterns.Add(ComboB);

	//ComboC
	FComboPattern ComboC;
	ComboC.ComboType = EComboType::SpinAttack;
	ComboC.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::Y,  EAttackKeyType::X };
	ComboC.ComboNum = 2;
	ComboC.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboC);
	ComboC.ComboAttackMultiplier = 3.5;
	ComboC.BPValue = 2;
	ComboPatterns.Add(ComboC);

	//ComboD
	FComboPattern ComboD;
	ComboD.ComboType = EComboType::UltimateCombo;
	ComboD.ComboKeySequence = { EAttackKeyType::Y, EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::Y };
	ComboD.ComboNum = 3;
	ComboD.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboD);
	ComboD.ComboAttackMultiplier = 3.8;
	ComboD.BPValue = 2;
	ComboPatterns.Add(ComboD);
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


//parameter: input key value from player (x/y)
bool UComboManageComponent::BossUltimateQTE(EAttackKeyType Key)
{
	InputKeySequence.Add(Key);
	TimeSinceLastInput = 0.f;

	GetWorld()->GetTimerManager().SetTimer(
		ComboTimeOutTimer, 
		this, &UComboManageComponent::CheckComboTimeOut, 
		ComboTimeOut, 
		false
	);

	for (int32 i = 0; i < ComboPatterns.Num(); i++)
	{
		//Compare InputSequence to Designated Combo Patterns
		if (InputKeySequence.Num() == ComboPatterns[RequiredBossComboNum].ComboKeySequence.Num())
		{
			bool bIsComboMatch = true;
			for (int32 j = 0; j < InputKeySequence.Num(); j++)
			{
				if (InputKeySequence[j] != ComboPatterns[RequiredBossComboNum].ComboKeySequence[j])
				{
					bIsComboMatch = false;
					break;
				}
			}
			if (bIsComboMatch)
			{
				//Set AttackMultiplier
				CurrentAttackMultiplier = ComboPatterns[RequiredBossComboNum].ComboAttackMultiplier;
				//start broadcast when combo matches
				OnComboMatch.Broadcast(ComboPatterns[RequiredBossComboNum].AnimMontageNum, ComboPatterns[RequiredBossComboNum].BPValue);
				UE_LOG(LogTemp, Warning, TEXT("ComboMatch!!"));
				ResetInputSequence();
				return bIsComboMatch;
			}
		}
	}
	return false;
}


