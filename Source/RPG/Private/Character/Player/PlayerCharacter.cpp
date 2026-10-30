#include "Character/Player/PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Animation/AnimMontage.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Component/Player/ComboManageComponent.h"
#include "Component/Player/HUDComponent.h"
#include "Component/Player/WeaponComponent.h"
#include "Component/Player/KickComponent.h"
#include "Component/Player/PlayerAttributeComponent.h"
#include "Component/Player/UltimateSkillComponent.h"
#include "Component/Player/InteractionComponent.h"
#include "Component/Player/SFXComponent.h"
#include "Component/Player/DodgeComponent.h"
#include "Game/RPGGameMode.h"
#include "Kismet/GameplayStatics.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 1. Input Subsystem Setup
	SetupInputSubsystem();

	// 2. Cache Components (for performance, avoid repeated FindComponentByClass)
	CacheComponents();

	// 3. Bind Delegates (Animation, Combo, Attribute, HUD)
	BindDelegates();

	// 4. Initialize UI (HP/BP/Level)
	InitializeHUD();


	SetupPlayerInputComponent(InputComponent);
}

void APlayerCharacter::SetupInputSubsystem()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer());

		if (InputSubsystem)
		{
			// Map-based context priority
			// Default priority: Player > Saloon
			if (GetWorld()->GetMapName().Contains(TEXT("Saloon")))
			{
				InputSubsystem->AddMappingContext(PlayerContext, 0);
				InputSubsystem->AddMappingContext(SaloonContext, 1);
			}
			else
			{
				InputSubsystem->AddMappingContext(PlayerContext, 1); 
				InputSubsystem->AddMappingContext(SaloonContext, 0);
			}

			UE_LOG(LogTemp, Warning, TEXT("Both Contexts Loaded"));
		}
	}
}

void APlayerCharacter::CacheComponents()
{
	CachedAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	CachedComboManageComponent = FindComponentByClass<UComboManageComponent>();
	CachedWeaponComponent = FindComponentByClass<UWeaponComponent>();
	CachedKickComponent = FindComponentByClass<UKickComponent>();
	CachedUltimateSkillComponent = FindComponentByClass<UUltimateSkillComponent>();
	CachedHUDComponent = FindComponentByClass<UHUDComponent>();
	CachedInteractionComponent = FindComponentByClass<UInteractionComponent>();
	CachedAttributeComponent = FindComponentByClass<UPlayerAttributeComponent>();
	CachedSFXComponent = FindComponentByClass<USFXComponent>();
}

void APlayerCharacter::BindDelegates()
{
	if (CachedAnimInstance)
	{
		CachedAnimInstance->OnAttackMontageEnd.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
		CachedAnimInstance->OnMoveEnable.AddDynamic(this, &APlayerCharacter::SetMoveEnable);

		CachedAnimInstance->OnBladeLCollisionEnable.AddDynamic(this, &APlayerCharacter::SetBladeLCollisionEnable);
		CachedAnimInstance->OnBladeLCollisionDisable.AddDynamic(this, &APlayerCharacter::SetBladeLCollisionDisable);
		CachedAnimInstance->OnBladeRCollisionEnable.AddDynamic(this, &APlayerCharacter::SetBladeRCollisionEnable);
		CachedAnimInstance->OnBladeRCollisionDisable.AddDynamic(this, &APlayerCharacter::SetBladeRCollisionDisable);

		CachedAnimInstance->OnKickCollisionEnable.AddDynamic(this, &APlayerCharacter::SetKickCollisionEnable);
		CachedAnimInstance->OnKickCollisionDisable.AddDynamic(this, &APlayerCharacter::SetKickCollisionDisable);

		CachedAnimInstance->OnGameOver.AddDynamic(this, &APlayerCharacter::OpenGameOverLevel);

		CachedAnimInstance->OnInvincibleStart.AddDynamic(this, &APlayerCharacter::SetInvincibleStart);
		CachedAnimInstance->OnInvincibleEnd.AddDynamic(this, &APlayerCharacter::SetInvincibleEnd);
	}

	if (CachedComboManageComponent)
	{
		CachedComboManageComponent->OnComboMatch.AddDynamic(this, &APlayerCharacter::OnComboComplete);
		CachedComboManageComponent->OnChainAttack.AddDynamic(this, &APlayerCharacter::OnChainAttack);
		CachedComboManageComponent->OnQTEFailed.AddDynamic(this, &APlayerCharacter::OnQTEFailed);
	}

	if (CachedUltimateSkillComponent)
	{
		CachedUltimateSkillComponent->OnSkill1Attack.AddDynamic(this, &APlayerCharacter::OnUltSkillAAttack);
		CachedUltimateSkillComponent->OnUltTimeOver.AddDynamic(this, &APlayerCharacter::UltTimeOver);
	}

	if (CachedHUDComponent)
	{
		CachedHUDComponent->OnPlayerSkillConfirmed.AddDynamic(this, &APlayerCharacter::OnSkillConfirmed);
		CachedHUDComponent->OnWidgetsReady.AddDynamic(this, &APlayerCharacter::InitializeHUD);
	}

	if (CachedAttributeComponent)
	{
		CachedAttributeComponent->OnBPUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::UpdatePlayerBP);
		CachedAttributeComponent->OnHPUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::UpdatePlayerHP);
		CachedAttributeComponent->OnLevelUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::UpdateLevelText);
		CachedAttributeComponent->OnLevelUp.AddDynamic(this, &APlayerCharacter::OnLevelUp);
		CachedAttributeComponent->OnDamageCalculated.AddDynamic(this, &APlayerCharacter::OnDamageCalculated);
		CachedAttributeComponent->OnBPMax.AddDynamic(this, &APlayerCharacter::SetIsSkillEnable);
		CachedAttributeComponent->OnSkillTreeUISet.AddDynamic(this, &APlayerCharacter::OnSkillTreeSet);
		CachedAttributeComponent->OnSkillPointUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::SetSkillPointText);
	}

	if (ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnDialogueEvent.AddDynamic(CachedHUDComponent, &UHUDComponent::ShowDialogue);
		GM->OnSaveRequired.AddDynamic(CachedAttributeComponent, &UPlayerAttributeComponent::SavePlayerData);
	}
}

void APlayerCharacter::InitializeHUD()
{
	//Initialize HP&BP Bar & SkillTree
	CachedAttributeComponent->BroadcastInitialUIState();
	CachedHUDComponent->UpdateLevelText(CachedAttributeComponent->GetCurrentLevel());
}

void APlayerCharacter::OnAttackMontageEnded()
{
	SetPlayerCombatState(EPlayerCombatState::EPCS_NotAttacking);
	CachedWeaponComponent->InitializeActorsToIgnore();
	CachedKickComponent->InitializeActorsToIgnore();
}

//delegate callback function (when combo matches)
void APlayerCharacter::OnComboComplete(int32 ComboNum, int32 BPValue)
{
	const EAnimMontageType ComboMontage = static_cast<EAnimMontageType>(ComboNum);
	CachedAnimInstance->PlayComboAttackMontage(ComboMontage);
	CachedSFXComponent->PlayComboAttackSFX(ComboNum);
	
	CachedAttributeComponent->SetBPtoGet(BPValue);//Set BPValue to get
	UE_LOG(LogTemp, Warning, TEXT("Combo!!"));

	CachedHUDComponent->ShowComboPopUp(ComboNum);

	//For UltSkillC
	// Broadcast to enemy when combo matches & UltSkillC is activated
	if (CachedUltimateSkillComponent->IsSkillActivated() && 
		CachedAttributeComponent->GetUltSkill() == static_cast<int32>(EUltimateSkill::SkillC))
	{
		OnSkillCReady.Broadcast();
	}
}

void APlayerCharacter::OnChainAttack(int32 AnimNum, FName SectionName)
{
	CachedAnimInstance->PlayChainAttackMontage(AnimNum, SectionName);
	CachedSFXComponent->PlayChainAttackSFX(AnimNum, SectionName);
}

void APlayerCharacter::SetMoveEnable()
{
	SetPlayerMoveState(EPlayerMoveState::EPMS_MoveEnabled);
	UE_LOG(LogTemp, Display, TEXT("PlayerMoveEnableSet"));
}

void APlayerCharacter::SetBladeLCollisionEnable()
{
	CachedWeaponComponent->OnAttackCollisionEnable(0); //0 is for left
}

void APlayerCharacter::SetBladeLCollisionDisable()
{
	CachedWeaponComponent->OnAttackCollisionDisable(0);
}

void APlayerCharacter::SetBladeRCollisionEnable()
{
	CachedWeaponComponent->OnAttackCollisionEnable(1); //1 is for right
}

void APlayerCharacter::SetBladeRCollisionDisable()
{
	CachedWeaponComponent->OnAttackCollisionDisable(1);
}

void APlayerCharacter::SetKickCollisionEnable()
{
	CachedKickComponent->OnAttackCollisionEnable(-1);
}

void APlayerCharacter::SetKickCollisionDisable()
{
	CachedKickComponent->OnAttackCollisionDisable(-1);
}

void APlayerCharacter::UpdateBP(int32 CurrentBP, int32 MaxBP) //callback Func called by AttributeComp
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateBP Callback in PC"));
	CachedHUDComponent->UpdatePlayerBP(CurrentBP, MaxBP);
}

void APlayerCharacter::UpdateHP(int32 CurrentHP, int32 MaxHP)
{
	UE_LOG(LogTemp, Warning, TEXT("UpdateHP Callback in PC"));
	CachedHUDComponent->UpdatePlayerHP(CurrentHP, MaxHP);
}

//SetSkillTreeUI When Load Game (=Restart)
void APlayerCharacter::OnSkillTreeUISet(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills)
{
	CachedHUDComponent->SetSkillPointText(SkillPoint);
	CachedHUDComponent->InitializeSkillTreeUIFromSave(UltNum, ActivatedSkills);
}

void APlayerCharacter::OnLevelUp(int32 CurrentLevel, int32 SkillPoint)
{
	UpdateHP(CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP());
	CachedHUDComponent->SetSkillPointText(SkillPoint);
	CachedHUDComponent->ShowLevelUpPopUp(CurrentLevel);

	CachedSFXComponent->PlayLevelUpSFX();
}

void APlayerCharacter::OnDamageCalculated(int32 Damage) //for SkillB(HP Drain)
{
	if (CachedUltimateSkillComponent->IsSkillActivated() && 
		CachedAttributeComponent->GetUltSkill() == static_cast<int32>(EUltimateSkill::SkillB))
	{
		CachedAttributeComponent->ADDCurrentHP(Damage);
		UpdateHP(CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP());
	}
}

void APlayerCharacter::OnSkillPointUpdate(int32 SkillPoint)
{
	CachedHUDComponent->SetSkillPointText(SkillPoint);
}

void APlayerCharacter::UltTimeOver()
{
	CachedHUDComponent->HideSkillEffect();
}

// called when start game
// Initialize SkillTree based on SaveGameData
void APlayerCharacter::OnSkillTreeSet(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting)
{
	CachedAttributeComponent->SetUltSkill(UltNum);
	CachedUltimateSkillComponent->SetKeyBindingXInSkillA(Ult1XSetting);
	CachedUltimateSkillComponent->SetKeyBindingYInSkillA(Ult1YSetting);
	CachedHUDComponent->SetSkillTree(SkillPoint, UltNum, ActivatedSkills, Ult1XSetting, Ult1YSetting);
}

void APlayerCharacter::OnQTEFailed()
{
	CachedHUDComponent->HideQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());
	CachedAnimInstance->PlayAnimMontage(3); // Play death Anim
}

//Callback Func (from delegate at UltSkillComp)
void APlayerCharacter::OnUltSkillAAttack(int32 AnimNum, float AttackMultiplier)
{
	CachedAttributeComponent->SetAttackMultiplier(AttackMultiplier);
	const EAnimMontageType ComboMontage = static_cast<EAnimMontageType>(AnimNum);
	CachedAnimInstance->PlayComboAttackMontage(ComboMontage);
	CachedSFXComponent->PlayComboAttackSFX(AnimNum);
}

void APlayerCharacter::StartBossQTEMode()
{
	IsTrapped = true;
	//Set Random QTE Combo Num
	CachedComboManageComponent->SetRequiredBossComboNum();
	CachedComboManageComponent->StartQTETimer();

	CachedHUDComponent->ShowQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());
}

void APlayerCharacter::ShowStartPopUp()
{
	CachedHUDComponent->ShowStartPopUp();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//Disable Move Func When Player Is Trapped by Boss's Ult Skill
	if (IsTrapped)
	{
		return;
	}

	if (GetPlayerMoveState() == EPlayerMoveState::EPMS_MoveEnabled)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		//FRotationMatrix(YawRotation): Make Rotation Matrix using Z-Axis Rotate
		//GetUnitAxis(EAxis::X): Original Forward Vector
		//FVector Forward: adjusted Forward Vector("Forward" based on the direction you are looking)
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, MovementVector.Y);

		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, MovementVector.X);
	}
}


void APlayerCharacter::Jump()
{
	Super::Jump();
}


void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();

	AddControllerPitchInput(LookVector.Y); //look up&down
	AddControllerYawInput(LookVector.X); //look left&right

}

//X / Y Button
void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	const float AttackValue = Value.Get<float>();

	HandleUltimateSkillAttack(AttackValue); //for SkillA

	if (GetPlayerCombatState() != EPlayerCombatState::EPCS_NotAttacking)
	{
		return;
	}

	SetPlayerCombatState(EPlayerCombatState::EPCS_Attacking);
	//disable move input when player is attacking
	SetPlayerMoveState(EPlayerMoveState::EPMS_MoveDisabled);

	const EAttackKeyType InputType = GetAttackKeyType(AttackValue);

	//When Player Is Trapped by Boss's Ult Skill -> RandomComboQTE Start!
	if (IsTrapped)
	{
		HandleQTEAttack(InputType, AttackValue);
		return;
	}
		
	HandleNormalComboAttack(InputType);
}

void APlayerCharacter::HandleUltimateSkillAttack(float AttackValue)
{
	// If ultimate SkillA is active, use attack input as skill - specific combo trigger
	if (CachedUltimateSkillComponent->IsSkillActivated() &&
		CachedAttributeComponent->GetUltSkill() == static_cast<int32>(EUltimateSkill::SkillA))
	{
		CachedUltimateSkillComponent->Skill1(AttackValue);
	}
}

EAttackKeyType APlayerCharacter::GetAttackKeyType(float AttackValue)
{
	if (AttackValue == 1.f)
	{
		return EAttackKeyType::X;
	}

	return EAttackKeyType::Y;
}

void APlayerCharacter::HandleQTEAttack(EAttackKeyType InputType, float AttackValue)
{
	if (!CachedComboManageComponent->BossUltimateQTE(InputType))
	{
		//X Button
		if (AttackValue == 1.f)
		{
			CachedComboManageComponent->ChainAttackCount(InputType);
		}
		//Y Button
		else if (AttackValue == -1.f)
		{
			CachedComboManageComponent->ChainAttackCount(InputType);
		}
	}
	else
	{
		//Broadcast to Boss
		OnQTESuccess.Broadcast();
		IsTrapped = false;

		CachedHUDComponent->HideQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());
		CachedHUDComponent->NoticeToBreak();
	}
}

void APlayerCharacter::HandleNormalComboAttack(EAttackKeyType InputType)
{
	//send ENUM to InputSequence array (for Combo check)
	if (!CachedComboManageComponent->IsComboMatch(InputType))
	{
		CachedComboManageComponent->ChainAttackCount(InputType);
	}
	CachedAttributeComponent->SetAttackMultiplier(CachedComboManageComponent->GetCurrentAttackMultiplier());
}

void APlayerCharacter::Dodge(const FInputActionValue& Value)
{
	if (CachedComboManageComponent)
	{
		CachedComboManageComponent->StartComboTimer();
	}
	CachedAnimInstance->PlayAnimMontage(5); //PlayDodgeAnim
}

void APlayerCharacter::SkillStart()
{
	if (IsSkillEnable)
	{
		EUltimateSkill UltSkill = static_cast<EUltimateSkill>(CachedAttributeComponent->GetUltSkill());

		//SKill 1
		if (UltSkill == EUltimateSkill::SkillA)
		{
			CachedUltimateSkillComponent->TimerStart(&UltSkill);
			CachedUltimateSkillComponent->SetCachedComboPatters(CachedComboManageComponent->GetComboPatterns());
		}
		else if (UltSkill == EUltimateSkill::SkillB)
		{
			CachedUltimateSkillComponent->TimerStart(&UltSkill);
		}
		else if (UltSkill == EUltimateSkill::SkillC)
		{
			CachedUltimateSkillComponent->TimerStart(&UltSkill);
		}

		const int32 SkillNum = static_cast<int32>(UltSkill);

		CachedHUDComponent->ShowSkillEffect(SkillNum);
		CachedSFXComponent->PlayUltSkillSFX(SkillNum);
		CachedAttributeComponent->UseBPforUlt();

		UpdateBP(CachedAttributeComponent->GetCurrentBP(), CachedAttributeComponent->GetMaxBP());

		IsSkillEnable = false;
	}
}

//Open SkillTree UI through PlayerCharacter to check player's level
//Open SkillTree is only possible when level >= 2
void APlayerCharacter::OpenSkillTree()
{
	if (CachedAttributeComponent->GetCurrentLevel() >= 2)
	{
		CachedHUDComponent->ShowSkillTreeUI();
	}
}

void APlayerCharacter::StartDance()
{
	if (CachedAnimInstance)
	{
		CachedAnimInstance->PlayAnimMontage(4); //Play Dance Anim Montage
	}
}

//Callback Func for Skill Set (A Button in SkillTreeUI)
void APlayerCharacter::OnSkillConfirmed(ESkillConfirmType ConfirmType, int32 Index)
{
	switch (ConfirmType)
	{
	case ESkillConfirmType::UltimateSkill:
		CachedAttributeComponent->SetUltSkill(Index);
		break;

	case ESkillConfirmType::NormalSkill:
		if (CachedAttributeComponent->GetSkillPoint() > 0) 
		{
			CachedAttributeComponent->UpdateStatsBySkill(Index);
		}
		break;

	case ESkillConfirmType::SkillTimeUp:
		CachedUltimateSkillComponent->IncreaseSkillTime();
		break;

	case ESkillConfirmType::KeyBindingX:
		CachedUltimateSkillComponent->SetKeyBindingXInSkillA(Index);
		CachedAttributeComponent->SetUlt1XSetting(Index);
		break;

	case ESkillConfirmType::KeyBindingY:
		CachedUltimateSkillComponent->SetKeyBindingYInSkillA(Index);
		CachedAttributeComponent->SetUlt1YSetting(Index);
		break;
	}
}

void APlayerCharacter::InteractPopUp()
{
	CachedHUDComponent->ShowInteractPopUp();
}

void APlayerCharacter::DancePopUp()
{
	CachedHUDComponent->ShowDancePopUp();
}

void APlayerCharacter::HideInteractPopUp()
{
	CachedHUDComponent->HideInteractPopUp();
}

void APlayerCharacter::OpenGameOverLevel()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("GameOver")));
}

void APlayerCharacter::SetInvincibleStart()
{
	CachedAttributeComponent->InvincibleStart();
}

void APlayerCharacter::SetInvincibleEnd()
{
	CachedAttributeComponent->InvincibleEnd();
}

void APlayerCharacter::OnInteract()
{
	CachedInteractionComponent->InteractionStart();
}

void APlayerCharacter::SetIsSkillEnable()
{
	CachedHUDComponent->UpdatePlayerBP(CachedAttributeComponent->GetCurrentBP(), CachedAttributeComponent->GetMaxBP());
	CachedSFXComponent->PlayBPMaxSPX();
	CachedHUDComponent->ShowReadyToUseUlt();
	IsSkillEnable = true;
}

//automatically called by UnrealEngine 
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//MainInputContext
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Dodge);
		EnhancedInputComponent->BindAction(SkillActivation, ETriggerEvent::Triggered, this, &APlayerCharacter::SkillStart);
		EnhancedInputComponent->BindAction(PopUpOff, ETriggerEvent::Triggered, CachedHUDComponent, &UHUDComponent::HidePopUp);

		//Open SkillTree Screen
		EnhancedInputComponent->BindAction(SkillTreeOn, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenSkillTree);

		//Open Menu Screen
		EnhancedInputComponent->BindAction(MenuOn, ETriggerEvent::Triggered, CachedHUDComponent, &UHUDComponent::ShowMenuUI);

		//Interact
		EnhancedInputComponent->BindAction(Interact, ETriggerEvent::Triggered, this, &APlayerCharacter::OnInteract);
		//Hide Dialogue
		EnhancedInputComponent->BindAction(HideDialogue, ETriggerEvent::Triggered, CachedHUDComponent, &UHUDComponent::HideDialogue);

		//Saloon Event
		EnhancedInputComponent->BindAction(Dance, ETriggerEvent::Triggered, this, &APlayerCharacter::StartDance);

	}
}

void APlayerCharacter::HandleAttack(AActor* Target)
{
	CachedAttributeComponent->SetEnemyAttributeComponent(Target);
}

void APlayerCharacter::GetHit(const FVector& ImpactPoint, AActor* Target)
{
	if (PlayerCombatState != EPlayerCombatState::EPCS_Dead)
	{
		UE_LOG(LogTemp, Error, TEXT("Player Hit!!!"));
		CachedAttributeComponent->SetEnemyAttributeComponent(Target);
		CachedAttributeComponent->ApplyDamage();

		if (CachedAttributeComponent->GetCurrentHP() <= 0.f)
		{
			CachedAnimInstance->PlayAnimMontage(3); //Play Death Anim
			CachedSFXComponent->PlayDeathSFX();
			PlayerCombatState = EPlayerCombatState::EPCS_Dead;
		}
		else
		{
			CachedAnimInstance->PlayAnimMontage(2); //play Stun Anim
			CachedSFXComponent->PlayHitSFX();
		}
	}
	PlayerMoveState = EPlayerMoveState::EPMS_MoveDisabled;
}


