#include "Component/Boss/BossAttributeComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Component/Player/PlayerAttributeComponent.h"
#include "Character/Boss/Boss.h"
#include "Character/Boss/Types/BossStat.h"

UBossAttributeComponent::UBossAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UBossAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	CachePlayerAttribute();

	FBossStat* Row = BossStatTable->FindRow<FBossStat>(TEXT("Boss"), TEXT("Load Boss Stat Data"));
	if (Row)
	{
		DEF = Row->DEF;
		CurrentHP = Row->CurrentHP;
		MaxHP = Row->MaxHP;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BossStatTable Load Failed"));
	}
}

void UBossAttributeComponent::CachePlayerAttribute()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		CachedPlayerAttributeComponent = Player->FindComponentByClass<UPlayerAttributeComponent>();
	}
}

int32 UBossAttributeComponent::CalcDamage()
{
	if (!CachedPlayerAttributeComponent)
	{
		return 0;
	}

	int32 BaseDamage = ATK - CachedPlayerAttributeComponent->GetDEF();
	int32 FinalDamage = FMath::Max(BaseDamage, 0);

	return FinalDamage;
}

void UBossAttributeComponent::ApplyDamage()
{
	if (!CachedPlayerAttributeComponent) return;

	int32 Damage = CachedPlayerAttributeComponent->CalcDamage();

	if (IsInfected)
	{
		Damage *= 2;
	}

	CurrentHP -= Damage;
	UE_LOG(LogTemp, Display, TEXT("BossHP: %d"), CurrentHP);


	// break crystals to show Boss's HP roughly
	if (CurrentHP < HP_THRESHOLD_2)
	{
		OnHpThreshold.Broadcast(2);
	}

	if (CurrentHP < HP_THRESHOLD_1)
	{
		OnHpThreshold.Broadcast(1);
	}
}

