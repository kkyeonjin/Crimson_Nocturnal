#include "Component/Player/PlayerAttributeComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/Boss/BossAttributeComponent.h"
#include "Component/Enemy/EnemyAttributeComponent.h"
#include "Game/RPGGameInstance.h"
#include "Game/RPGGameMode.h"
#include "Kismet/GameplayStatics.h"


UPlayerAttributeComponent::UPlayerAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UPlayerAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// 1. Subscribe to GameInstance events (loose coupling)
	SubscribeToGameInstance();

	// 2. Restore data (runtime > save > new game priority)
	if (!TryRestoreRuntimeData() && !TryLoadSavedData())
	{
		InitializePlayerStat();
	}

	// 3. Bind GameMode events
	BindGameModeEvents();
}

void UPlayerAttributeComponent::SubscribeToGameInstance()
{
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->OnSaveGameRequested.AddDynamic(this, &UPlayerAttributeComponent::SavePlayerData);
		GI->OnLoadGameRequested.AddDynamic(this, &UPlayerAttributeComponent::LoadPlayerData);
		GI->OnRuntimeRestoreRequested.AddDynamic(this, &UPlayerAttributeComponent::RestoreRunTimeData);
	}
}

bool UPlayerAttributeComponent::TryRestoreRuntimeData()
{
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (GI->HasRunTimeSaveData())
		{
			GI->RestorePlayerData();
			UE_LOG(LogTemp, Warning, TEXT("Runtime restore complete"));
			return true;
		}
	}
	return false;
}

bool UPlayerAttributeComponent::TryLoadSavedData()
{
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (GI->HasSaveData())
		{
			LoadPlayerData();
			return true;
		}
	}
	return false;
}

void UPlayerAttributeComponent::BindGameModeEvents()
{
	if (ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnSaveRequired.AddDynamic(this, &UPlayerAttributeComponent::RequestSave);
	}
}

void UPlayerAttributeComponent::BroadcastInitialUIState()
{
	OnBPUpdate.Broadcast(CurrentBP, MaxBP);
	OnHPUpdate.Broadcast(CurrentHP, MaxHP);
	OnSkillTreeUISet.Broadcast(SkillPoint, EquippedUlt, ActivatedSkills, Ult1XSetting, Ult1YSetting);
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

		if (CurrentBP == MaxBP)
		{
			IsBPMax = true;
		}
		else
		{
			IsBPMax = false;
		}
	}
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
	NewPlayerData->IsTutorialChecked = true;

	UGameplayStatics::SaveGameToSlot(NewPlayerData, TEXT("PlayerSaveSlot"), 0);
}

// Called in runtime (before teleport to another level)
void UPlayerAttributeComponent::RequestSave()
{
	SavePlayerData();

	// BackUp runtime data in GI
	if (URPGGameInstance* GI = Cast<URPGGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->BackupPlayerData(); 
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


	ActivatedSkills.Empty();
	ActivatedSkills.SetNum(5, false); 
	if (RuntimeSaveGameData->ActivatedSkills.IsValidIndex(0) && RuntimeSaveGameData->ActivatedSkills.Num() >= 5)
	{
		ActivatedSkills = RuntimeSaveGameData->ActivatedSkills;
	}
	else
	{
		ActivatedSkills.Init(0, 5);
		UE_LOG(LogTemp, Warning, TEXT("Restore: ActivatedSkills reset to default (empty save)"));
	}

	BroadcastInitialUIState();
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

		if (CurrentBP >= MaxBP)
		{
			CurrentBP = MaxBP;
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
	int32 Damage = 0;

	if (EnemyAttributeComponent)
	{
		Damage = EnemyAttributeComponent->CalcDamage();
	}
	else if (BossAttributeComponent)
	{
		Damage = BossAttributeComponent->CalcDamage();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyAttributeComponent CANNOT FIND"));
		return;
	}

	CurrentHP -= Damage;
	OnHPUpdate.Broadcast(CurrentHP, MaxHP);
}

void UPlayerAttributeComponent::SetEnemyAttributeComponent(AActor* Target)
{
	if (!Target) return;

	// Priority: Boss > Enemy
	if (UBossAttributeComponent* BossComp = Target->FindComponentByClass<UBossAttributeComponent>())
	{
		EnemyAttributeComponent = nullptr;  
		BossAttributeComponent = BossComp;  
		TargetDEF = BossAttributeComponent->GetDEF();
		return;
	}

	if (UEnemyAttributeComponent* EnemyComp = Target->FindComponentByClass<UEnemyAttributeComponent>())
	{
		EnemyAttributeComponent = EnemyComp;
		BossAttributeComponent = nullptr;  
		TargetDEF = EnemyAttributeComponent->GetDEF();
		return;
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
	case 0: 
		ATK += 3;
		break;
	case 1: 
		DEF += 4;
		break;
	case 2: 
		MaxHP += 30;
		OnHPUpdate.Broadcast(CurrentHP, MaxHP);
		break;
	case 3: 
		BPMultiplier = 1.3f;
		break;
	default:
		break;
	}

	SkillPoint -= 1;

	//UI Update(SkillPoint Text)
	OnSkillPointUpdate.Broadcast(SkillPoint);

	ActivatedSkills[SkillNum] = 1;
}
