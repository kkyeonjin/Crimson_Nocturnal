// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/PlayerAttributeComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/Boss/BossAttributeComponent.h"
#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Game/RPGGameInstance.h"
#include "Game/RPGGameMode.h"
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
		GI->OnRuntimeRestoreRequested.AddDynamic(this, &UPlayerAttributeComponent::RestoreRunTimeData);
	}

	// 2. 런타임 백업 우선 복원 (맵 전환용)
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (GI->HasRunTimeSaveData()) {
			GI->RestorePlayerData(); 
			// 스탯 복원 + UI Broadcast 내부 처리
			UE_LOG(LogTemp, Warning, TEXT("Runtime restore complete"));
			return;  // 끝!
		}
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

	if (ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnSaveRequired.AddDynamic(this, &UPlayerAttributeComponent::RequestSave);
	}

	//여기에서 UI setting 델리게이트!!
	OnSkillTreeUISet.Broadcast(SkillPoint, EquippedUlt, ActivatedSkills, Ult1XSetting, Ult1YSetting);
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
	EquippedUlt = 10;
	ActivatedSkills = { 0, 0, 0, 0, 0 };

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

	if (CurrentLevel == 2)
	{
		SkillPoint += 2;
	}
	else
	{
		SkillPoint += 1;
	}

	OnLevelUp.Broadcast(CurrentLevel, SkillPoint);
	OnLevelUpdate.Broadcast(CurrentLevel);
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

	UE_LOG(LogTemp, Warning, TEXT("LoadPlayerData in AttributeComp"));

	//여기에서 UI setting 델리게이트!!
	OnBPUpdate.Broadcast(CurrentBP, MaxBP);
	OnHPUpdate.Broadcast(CurrentHP, MaxHP);
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

	UGameplayStatics::SaveGameToSlot(NewPlayerData, TEXT("PlayerSaveSlot"), 0);

}

void UPlayerAttributeComponent::RequestSave()
{
	SavePlayerData();

	// 2. GI 백업 (1줄)
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->BackupPlayerData(); // GI.CurrentPlayerSaveBackup 저장
	}
}

void UPlayerAttributeComponent::RestoreRunTimeData(USaveGameData* RuntimeSaveGameData)
{
	CurrentEXP = RuntimeSaveGameData->SavedCurrentExp;
	CurrentBP = RuntimeSaveGameData->SavedCurrentBP;
	CurrentHP = RuntimeSaveGameData->SavedCurrentHP;
	MaxBP = RuntimeSaveGameData->SavedMaxBP;
	MaxHP = RuntimeSaveGameData->SavedMaxHP;
	ATK = RuntimeSaveGameData->SavedATK ;
	DEF = RuntimeSaveGameData->SavedDEF;
	SkillPoint = RuntimeSaveGameData->SavedSkillPoint;
	CurrentLevel = RuntimeSaveGameData->SavedCurrentLevel;
	EquippedUlt = RuntimeSaveGameData->EquippedUlt;
	ActivatedSkills = RuntimeSaveGameData->ActivatedSkills;
	Ult1XSetting = RuntimeSaveGameData->Ult1XSetting;
	Ult1YSetting = RuntimeSaveGameData->Ult1YSetting;

	//여기에서 UI setting 델리게이트!!
	OnBPUpdate.Broadcast(CurrentBP, MaxBP);
	OnHPUpdate.Broadcast(CurrentHP, MaxHP);
	OnSkillTreeUISet.Broadcast(SkillPoint, EquippedUlt, ActivatedSkills, Ult1XSetting, Ult1YSetting);
	
}

//Calculation Damage Value to Enemy when Player hits Enemy
int32 UPlayerAttributeComponent::CalcDamage()
{
	if (BPtoGet != 0 && !IsBPMax)
	{
		UE_LOG(LogTemp, Display, TEXT("BPtoGet: %d"), BPtoGet);
		CurrentBP += BPtoGet;
		UE_LOG(LogTemp, Display, TEXT("CurrentBP: %d"), CurrentBP);
		OnBPUpdate.Broadcast(CurrentBP, MaxBP);

		if (CurrentBP == MaxBP)
		{
			OnBPMax.Broadcast();
			IsBPMax = true;
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
