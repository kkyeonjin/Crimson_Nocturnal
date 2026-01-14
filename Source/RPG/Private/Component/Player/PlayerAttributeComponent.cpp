// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/PlayerAttributeComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Kismet/GameplayStatics.h"


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

	if (DoesSaveGameExist())
	{
		LoadPlayerData();
	}
	else
	{
		//Initialize player stat
		InitializePlayerStat();
		UE_LOG(LogTemp, Warning, TEXT("Initialize Player Stat"));
	}

	//OnBPUpdate.Broadcast(CurrentBP, MaxBP);
	//OnHPUpdate.Broadcast(CurrentHP, MaxHP);
}

void UPlayerAttributeComponent::InitializePlayerStat()
{
	CurrentLevel = 1;
	CurrentBP = 0;
	CurrentEXP = 0;
	FLevelData* Data = GetLevelData(CurrentLevel);
	CurrentHP = Data->MaxHP;
	MaxHP = Data->MaxHP;
	ATK = Data->DefaultATK;
	DEF = Data->DefaultDEF;

	UE_LOG(LogTemp, Warning, TEXT("CurrentBP: %d MaxBP: %d CurrentHP: %d MaxHP: %d"), CurrentBP, MaxBP, CurrentHP, MaxHP);
}

FLevelData* UPlayerAttributeComponent::GetLevelData(int32 Level)
{
	if (!PlayerLevelTable)
	{
		return nullptr;
	}
	FName Row = FName(*FString::Printf(TEXT("Level%d"), Level));
	return PlayerLevelTable->FindRow<FLevelData>(Row, TEXT("Load Player Level Data"));
}

void UPlayerAttributeComponent::CheckLevelUp()
{
	FLevelData* Data = GetLevelData(CurrentLevel + 1);

	if (CurrentEXP >= Data->RequiredEXP)
	{
		LevelUp();
	}
}

void UPlayerAttributeComponent::LevelUp()
{
	CurrentLevel += 1;

	FLevelData* Data = GetLevelData(CurrentLevel);

	MaxHP = Data->MaxHP;
	CurrentHP = Data->MaxHP; //restore HP when LevelUP
	ATK = Data->DefaultATK;
	DEF = Data->DefaultDEF;
	SkillPoint += 1;

	OnLevelUp.Broadcast(SkillPoint);
}

void UPlayerAttributeComponent::LoadPlayerData()
{
	auto PlayerGameData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));

	if (PlayerGameData)
	{
		CurrentEXP = PlayerGameData->SavedCurrentExp;
		CurrentLevel = PlayerGameData->SavedCurrentLevel;
		CurrentBP = PlayerGameData->SavedCurrentBP;
		CurrentHP = PlayerGameData->SavedCurrentHP;
		MaxBP = PlayerGameData->SavedMaxBP;
		MaxHP = PlayerGameData->SavedMaxHP;
		ATK = PlayerGameData->SavedATK;
		DEF = PlayerGameData->SavedDEF;
		SkillPoint = PlayerGameData->SavedSkillPoint;
		ActivatedSkills = PlayerGameData->ActivatedSkills;
		EquippedUlt = PlayerGameData->EquippedUlt;
	}

	OnSkillTreeUISet.Broadcast(SkillPoint, EquippedUlt, ActivatedSkills);
}

void UPlayerAttributeComponent::SavePlayerData()
{
	USaveGameData* NewPlayerData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));

	if (!NewPlayerData)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Create SaveGame Obj"));
	}

	NewPlayerData->SavedCurrentExp = CurrentEXP;
	NewPlayerData->ActivatedSkills = ActivatedSkills;
	NewPlayerData->SavedCurrentBP = CurrentBP;
	NewPlayerData->SavedCurrentHP = CurrentHP;
	NewPlayerData->SavedMaxBP = MaxBP;
	NewPlayerData->SavedMaxHP = MaxHP;
	NewPlayerData->SavedATK = ATK;
	NewPlayerData->SavedDEF = DEF;
	NewPlayerData->SavedSkillPoint = SkillPoint;
	NewPlayerData->SavedCurrentLevel = CurrentLevel;
	NewPlayerData->EquippedUlt = EquippedUlt;

	UGameplayStatics::SaveGameToSlot(NewPlayerData, NewPlayerData->SaveSlotName, NewPlayerData->UserIndex);
}

bool UPlayerAttributeComponent::DoesSaveGameExist() const
{
	return UGameplayStatics::DoesSaveGameExist(TEXT("PlayerSaveSlot"), 0);
}

//Calculation Damage Value to Enemy when Player hits Enemy
int32 UPlayerAttributeComponent::CalcDamage()
{
	if (BPtoGet != 0)
	{
		UE_LOG(LogTemp, Display, TEXT("BPtoGet: %d"), BPtoGet);
		CurrentBP += BPtoGet;
		UE_LOG(LogTemp, Display, TEXT("CurrentBP: %d"), CurrentBP);
		OnBPUpdate.Broadcast(CurrentBP, MaxBP);

		BPtoGet = 0;
	}

	if (!EnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("EAC is not Found"));
		return 0.f;
	}

	int32 Damage = ATK - EnemyAttributeComponent->GetDEF();
	if (Damage > 0)
	{
		OnDamageCalculated.Broadcast(Damage); //for skillB
		return Damage;
	}
	else
	{
		return 0;
	}
}

//Apply Damage(=Reduce Player's HP) that Enemy gave to Player
void UPlayerAttributeComponent::ApplyDamage()
{
	if (EnemyAttributeComponent)
	{
		int32 Damage = EnemyAttributeComponent->CalcDamage();
		CurrentHP -= Damage;
		OnHPUpdate.Broadcast(CurrentHP, MaxHP);
		UE_LOG(LogTemp, Display, TEXT("PlayerHP: %d"), CurrentHP);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyAttributeComponent CANNOT FIND"));
	}
}

void UPlayerAttributeComponent::SetEnemyAttributeComponent(AActor* Target)
{
	EnemyAttributeComponent = Target->FindComponentByClass<UEnemyAttributeComponent>();
	if (!EnemyAttributeComponent)
	{
		UE_LOG(LogTemp, Display, TEXT("EnemyAttributeComp Cannot Find"));
	}
}

//take Enemy's EXP value who player just killed 
void UPlayerAttributeComponent::AddEXP(int32 EXPValue)
{
	if (CurrentLevel >= MaxLevel)
	{
		return;
	}

	CurrentEXP += EXPValue;

	CheckLevelUp();

	UE_LOG(LogTemp, Display, TEXT("EXP: %d"), CurrentEXP);
}

void UPlayerAttributeComponent::UpdateStatsBySkill(int SkillNum)
{
	switch (SkillNum)
	{
	case 0: //공격력 증가
		ATK *= 1.2f;
		break;
	case 1: //방어력 증가
		DEF *= 1.2f;
		break;
	case 2: //최대HP증가(바를 늘려야할텐데 그게 귀찮은,,) or 낮은 확률로 공격받아도 대미지 입지 않음
		MaxHP *= 1.2f;
		break;
	case 3: //BP획득량 증가
		BPMultiplier = 1.2f;
		break;
	default:
		break;
	}

	SkillPoint -= 1;
	OnSkillPointUpdate.Broadcast(SkillPoint);

	ActivatedSkills[SkillNum] = 1;
}
