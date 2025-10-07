// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyComponents/EnemyAttributeComponent.h"
#include "Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PlayerComponents/PlayerAttributeComponent.h"

// Sets default values for this component's properties
UEnemyAttributeComponent::UEnemyAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter)
	{
		PlayerAttributeComponent = PlayerCharacter->FindComponentByClass<UPlayerAttributeComponent>();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter From EnemyComp Load Failed"));
	}

	FBaseCharacterStat* Row = EnemyStatTable->FindRow<FBaseCharacterStat>(FName(TEXT("Mutant")), TEXT("Load Enemy Stat Data"));
	if (Row)
	{
		ATK = Row->ATK;
		DEF = Row->DEF;
		CurrentHP = Row->CurrentHP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ENemyStatTable Load Failed"));
	}
	// ...
	
}

void UEnemyAttributeComponent::InitializeEnemyStat()
{
}


// Called every frame
void UEnemyAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UEnemyAttributeComponent::CalcDamage()
{
	return 0.0f;
}

void UEnemyAttributeComponent::ApplyDamage()
{
	if (PlayerAttributeComponent)
	{
		float Damage = PlayerAttributeComponent->CalcDamage();
		CurrentHP -= Damage;
		UE_LOG(LogTemp, Display, TEXT("EnemyHP: %f"), CurrentHP);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PAC CANNOT FIND"));
	}
}

