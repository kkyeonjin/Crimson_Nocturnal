// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerComponents/PlayerAttributeComponent.h"
#include "EnemyComponents/EnemyAttributeComponent.h"

// Sets default values for this component's properties
UPlayerAttributeComponent::UPlayerAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	FPlayerStat* Row = PlayerStatTable->FindRow<FPlayerStat>(FName(TEXT("MainPlayer")), TEXT("Load Player Stat Data"));
	if (Row)
	{
		ATK = Row->ATK;
		DEF = Row->DEF;
		CurrentHP = Row->CurrentHP;
	}
	// ...
	
}

void UPlayerAttributeComponent::InitializePlayerStat()
{
	//PlayerStat.
}


// Called every frame
void UPlayerAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UPlayerAttributeComponent::CalcDamage()
{
	if (!EnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("EAC is not Found"));
		return 0.f;
	}
	float EnemyDEF = EnemyAttributeComponent->GetDEF();
	return ATK - EnemyDEF;
}

void UPlayerAttributeComponent::ApplyDamage()
{

}

void UPlayerAttributeComponent::SetEnemyAttributeComponent(AActor* Target)
{
	EnemyAttributeComponent = Target->FindComponentByClass<UEnemyAttributeComponent>();
	if (!EnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("EnemyAttributeComp Cannot Find"));
	}
	UE_LOG(LogTemp, Display, TEXT("EnemyAttributeComp Find"));
}

