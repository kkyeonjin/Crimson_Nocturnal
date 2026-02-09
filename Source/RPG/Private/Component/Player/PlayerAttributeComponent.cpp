// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/PlayerAttributeComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/Boss/BossAttributeComponent.h"
#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Game/RPGGameInstance.h"
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

	// GameInstance 델리게이트 구독 (느슨한 결합)
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->OnSaveGameRequested.AddDynamic(this, &UPlayerAttributeComponent::SavePlayerData);
		GI->OnLoadGameRequested.AddDynamic(this, &UPlayerAttributeComponent::LoadPlayerData);
	}

	// 저장 있으면 자동 로드
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (GI->HasSaveData())
		{
			LoadPlayerData();
		}
		else
		{
			InitializePlayerStat();
		}
	}

	OnBPUpdate.Broadcast(CurrentBP, MaxBP);
	OnHPUpdate.Broadcast(CurrentHP, MaxHP);
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

	OnLevelUp.Broadcast(CurrentLevel, SkillPoint);
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
		Ult1XSetting = PlayerGameData->Ult1XSetting;
		Ult1YSetting = PlayerGameData->Ult1YSetting;
	}

	OnSkillTreeUISet.Broadcast(SkillPoint, EquippedUlt, ActivatedSkills, Ult1XSetting, Ult1YSetting);
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
	NewPlayerData->ActivatedSkills = ActivatedSkills;
	NewPlayerData->Ult1XSetting = Ult1XSetting;
	NewPlayerData->Ult1YSetting = Ult1YSetting;
	NewPlayerData->CurrentMap = GetWorld()->GetMapName();

	UGameplayStatics::SaveGameToSlot(NewPlayerData, NewPlayerData->SaveSlotName, NewPlayerData->UserIndex);
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

		if (CurrentBP == MaxBP)
		{
			OnBPMax.Broadcast();
		}

		BPtoGet = 0;
	}

	int32 Damage = ATK - TargetDEF;
	if (Damage > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Damage!! :%d"), Damage);
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
	else if (BossAttributeComponent)
	{
		int32 Damage = BossAttributeComponent->CalcDamage();
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
	if (!Target) return;

	// 1순위: BossAttributeComponent 확인
	if (UBossAttributeComponent* BossComp = Target->FindComponentByClass<UBossAttributeComponent>())
	{
		EnemyAttributeComponent = nullptr;  // Enemy 타입은 Clear
		BossAttributeComponent = BossComp;  // Boss 타입 저장

		UE_LOG(LogTemp, Display, TEXT("Found BossAttributeComponent"));
		TargetDEF = BossAttributeComponent->GetDEF();
		return;
	}

	// 2순위: EnemyAttributeComponent 확인
	if (UEnemyAttributeComponent* EnemyComp = Target->FindComponentByClass<UEnemyAttributeComponent>())
	{
		EnemyAttributeComponent = EnemyComp;
		BossAttributeComponent = nullptr;  // Boss 타입은 Clear

		UE_LOG(LogTemp, Display, TEXT("Found EnemyAttributeComponent"));
		TargetDEF = EnemyAttributeComponent->GetDEF();
		return;
	}

	// 둘 다 없음
	UE_LOG(LogTemp, Warning, TEXT("No Attribute Component found on %s"), *Target->GetName());
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
		ATK += 3;
		break;
	case 1: //방어력 증가
		DEF += 4;
		break;
	case 2: //최대HP증가
		MaxHP += 30;
		break;
	case 3: //BP획득량 증가
		BPMultiplier = 1.3f;
		break;
	default:
		break;
	}

	SkillPoint -= 1;
	OnSkillPointUpdate.Broadcast(SkillPoint);
	//HP 표시량(숫자) 변화하도록 델리게이트 브로드캐스팅 해주기

	ActivatedSkills[SkillNum] = 1;
}
