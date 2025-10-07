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



void UComboManageComponent::AddInputKey(EAttackKeyType Key) //이름다시지을필요가존나있음 
{
	InputKeySequence.Add(Key);
	TimeSinceLastInput = 0.f;

	GetWorld()->GetTimerManager().SetTimer(
		ComboTimeOutTimer, this, &UComboManageComponent::CheckComboTimeOut, ComboTimeOut, false
	); //last parameter false-> call Timer just once 

	//UE_LOG(LogTemp, Warning, TEXT("InputKeySequence"));

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
				OnComboComplete.Broadcast(ComboPatterns[i].AnimMontageNum);
				UE_LOG(LogTemp, Warning, TEXT("ComboMatch!!"));
				ResetInputSequence();
				break;
			}
		}
	}
}

FName UComboManageComponent::ChainAttackCount(EAttackKeyType Key)
{
	FString SectionNameStr;

	if (Key == EAttackKeyType::X)
	{
		//UE_LOG(LogTemp, Display, TEXT("CountX : %d"), ChainAttackCountX);
		if (ChainAttackCountX == 3)
		{
			ChainAttackCountX = 0;
		}
		ChainAttackCountX++;
		UE_LOG(LogTemp, Display, TEXT("CountX : %d"), ChainAttackCountX);
		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountX);
	}
	else if (Key == EAttackKeyType::Y)
	{
		if (ChainAttackCountY == 2)
		{
			ChainAttackCountY = 0;
		}
		ChainAttackCountY++;
		UE_LOG(LogTemp, Display, TEXT("CountY : %d"), ChainAttackCountX);
		SectionNameStr = FString::Printf(TEXT("Chain%d"), ChainAttackCountY);
	}
	FName SectionName(*SectionNameStr);
	return SectionName;
}

void UComboManageComponent::InitializeComboPattern()
{
	//ComboA
	FComboPattern ComboA;
	ComboA.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::X, EAttackKeyType::Y };
	ComboA.ComboNum = 0;
	ComboA.AnimMontageNum = 10;
	ComboPatterns.Add(ComboA);

	//ComboB
	FComboPattern ComboB;
	ComboB.ComboKeySequence = { EAttackKeyType::X, EAttackKeyType::Y, EAttackKeyType::X };
	ComboB.ComboNum = 1;
	ComboB.AnimMontageNum = 11;
	ComboPatterns.Add(ComboB);
}

void UComboManageComponent::CheckComboTimeOut()
{
	//TimeSinceLastInput += DeltaTime;

	ResetInputSequence();
	ChainAttackCountX = 0;
	ChainAttackCountY = 0;
	UE_LOG(LogTemp, Warning, TEXT("InputReset"));

	/*
	if (TimeSinceLastInput > ComboTimeOut)
	{
		ResetInputSequence();
		ChainAttackCountX = 0;
		ChainAttackCountY = 0;
		UE_LOG(LogTemp, Warning, TEXT("InputReset"));
	}
	*/
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