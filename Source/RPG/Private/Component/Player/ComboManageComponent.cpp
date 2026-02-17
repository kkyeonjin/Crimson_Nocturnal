#include "Component/Player/ComboManageComponent.h"


UComboManageComponent::UComboManageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	InitializeComboPattern();
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
	return ProcessComboInput(Key, /*useAllPatterns=*/true);
}

bool UComboManageComponent::BossUltimateQTE(EAttackKeyType Key)
{
	return ProcessComboInput(Key, /*useAllPatterns=*/false);
}

// Core combo matching logic(shared by normal / QTE)
// bUseAllPatterns True for normal combo scan, false for boss QTE specific
bool UComboManageComponent::ProcessComboInput(EAttackKeyType Key, bool bUseAllPatterns)
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

	const int32 TargetPatternIndex = bUseAllPatterns ? -1 : RequiredBossComboNum;
	const int32 PatternCount = bUseAllPatterns ? ComboPatterns.Num() : 1;


	for (int32 i = 0; i < PatternCount; ++i)
	{
		const int32 PatternIdx = bUseAllPatterns ? i : TargetPatternIndex;
		const FComboPattern& Pattern = ComboPatterns[PatternIdx];

		if (InputKeySequence.Num() == Pattern.ComboKeySequence.Num())
		{
			bool bIsMatch = true;
			for (int32 j = 0; j < InputKeySequence.Num(); ++j)
			{
				if (InputKeySequence[j] != Pattern.ComboKeySequence[j])
				{
					bIsMatch = false;
					break;
				}
			}

			if (bIsMatch)
			{
				CurrentAttackMultiplier = Pattern.ComboAttackMultiplier;

				// Clear timers on success
				GetWorld()->GetTimerManager().ClearTimer(ComboTimeOutTimer);
				if (!bUseAllPatterns)
				{
					GetWorld()->GetTimerManager().ClearTimer(BossSurvivalTimer);
				}

				// Broadcast (QTE uses AnimMontageNum for combo num)
				OnComboMatch.Broadcast(bUseAllPatterns ? Pattern.ComboNum : Pattern.AnimMontageNum, Pattern.BPValue);
				UE_LOG(LogTemp, Warning, TEXT("ComboMatch!!"));

				ResetInputSequence();
				return true;
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

		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountY);
		AnimNum = 1;

		//Set CurrentAttackMultiplier for each ChainCountSteps
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
	ComboA.BPValue = 20;
	ComboPatterns.Add(ComboA);

	//ComboB
	FComboPattern ComboB;
	ComboB.ComboType = EComboType::AdvancedCombo;
	ComboB.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::X };
	ComboB.ComboNum = 1;
	ComboB.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboB);
	ComboB.ComboAttackMultiplier = 2.8;
	ComboB.BPValue = 25;
	ComboPatterns.Add(ComboB);

	//ComboC
	FComboPattern ComboC;
	ComboC.ComboType = EComboType::SpinAttack;
	ComboC.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::Y,  EAttackKeyType::X };
	ComboC.ComboNum = 2;
	ComboC.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboC);
	ComboC.ComboAttackMultiplier = 3.5;
	ComboC.BPValue = 30;
	ComboPatterns.Add(ComboC);

	//ComboD
	FComboPattern ComboD;
	ComboD.ComboType = EComboType::UltimateCombo;
	ComboD.ComboKeySequence = { EAttackKeyType::Y, EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::Y };
	ComboD.ComboNum = 3;
	ComboD.AnimMontageNum = static_cast<int32>(EAnimMontageType::ComboD);
	ComboD.ComboAttackMultiplier = 3.8;
	ComboD.BPValue = 35;
	ComboPatterns.Add(ComboD);
}

void UComboManageComponent::CheckComboTimeOut()
{
	ResetInputSequence();
	ChainAttackCountX = 0;
	ChainAttackCountY = 0;
	UE_LOG(LogTemp, Warning, TEXT("InputReset"));
}

void UComboManageComponent::StartQTETimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		BossSurvivalTimer,
		this, &UComboManageComponent::QTEFailed,
		BossSurvivalTimeOut,
		false
	);
}


void UComboManageComponent::QTEFailed()
{
	OnQTEFailed.Broadcast();
}

