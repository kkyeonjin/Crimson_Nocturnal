// Fill out your copyright notice in the Description page of Project Settings.


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
#include "Game/RPGGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SkillTreeWidget.h"


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

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			PlayerController->GetLocalPlayer());

		if (InputSubsystem)
		{
			//둘 다 추가 (Saloon은 Priority 낮게)
			if (GetWorld()->GetMapName().Contains(TEXT("Saloon")))
			{
				InputSubsystem->AddMappingContext(PlayerContext, 0);  
				InputSubsystem->AddMappingContext(SaloonContext, 1);
			}
			else
			{
				InputSubsystem->AddMappingContext(PlayerContext, 1);  // 기본 Priority 높게
				InputSubsystem->AddMappingContext(SaloonContext, 0);
			}

			UE_LOG(LogTemp, Warning, TEXT("Both Contexts Loaded"));
		}
	}

	//cast animInstance In here (for reuse)
	CachedAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	CachedComboManageComponent = FindComponentByClass<UComboManageComponent>();
	CachedWeaponComponent = FindComponentByClass<UWeaponComponent>();
	CachedKickComponent = FindComponentByClass<UKickComponent>();
	CachedUltimateSkillComponent = FindComponentByClass<UUltimateSkillComponent>();
	CachedHUDComponent = FindComponentByClass<UHUDComponent>();
	CachedInteractionComponent = FindComponentByClass<UInteractionComponent>();
	CachedAttributeComponent = FindComponentByClass<UPlayerAttributeComponent>();
	CachedSFXComponent = FindComponentByClass<USFXComponent>();


	if (CachedAnimInstance)
	{
		//binding to MontageEnd Delegate 
		CachedAnimInstance->OnAttackMontageEnd.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
		CachedAnimInstance->OnMoveEnable.AddDynamic(this, &APlayerCharacter::SetMoveEnable);

		CachedAnimInstance->OnBladeLCollisionEnable.AddDynamic(this, &APlayerCharacter::SetBladeLCollisionEnable);
		CachedAnimInstance->OnBladeLCollisionDisable.AddDynamic(this, &APlayerCharacter::SetBladeLCollisionDisable);
		CachedAnimInstance->OnBladeRCollisionEnable.AddDynamic(this, &APlayerCharacter::SetBladeRCollisionEnable);
		CachedAnimInstance->OnBladeRCollisionDisable.AddDynamic(this, &APlayerCharacter::SetBladeRCollisionDisable);

		CachedAnimInstance->OnKickCollisionEnable.AddDynamic(this, &APlayerCharacter::SetKickCollisionEnable);
		CachedAnimInstance->OnKickCollisionDisable.AddDynamic(this, &APlayerCharacter::SetKickCollisionDisable);

		CachedAnimInstance->OnGameOver.AddDynamic(this, &APlayerCharacter::OpenGameOverLevel);
	}

	if (CachedComboManageComponent)
	{
		CachedComboManageComponent->OnComboMatch.AddDynamic(this, &APlayerCharacter::OnComboComplete);
		CachedComboManageComponent->OnChainAttack.AddDynamic(this, &APlayerCharacter::OnChainAttack);
		CachedComboManageComponent->OnQTEFailed.AddDynamic(this, &APlayerCharacter::OnQTEFailed);
	}

	if (CachedUltimateSkillComponent)
	{
		CachedUltimateSkillComponent->OnSkill1Attack.AddDynamic(this, &APlayerCharacter::OnSkill1Attack);
		CachedUltimateSkillComponent->OnUltTimeOver.AddDynamic(this, &APlayerCharacter::UltTimeOver);
	}
	
	if (CachedHUDComponent)
	{
		//Initialize HP&BP Bar
		UpdateHP(CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP());
		UpdateBP(CachedAttributeComponent->GetCurrentBP(), CachedAttributeComponent->GetMaxBP());
		CachedHUDComponent->UpdateLevelText(CachedAttributeComponent->GetCurrentLevel());

		CachedHUDComponent->OnPlayerSkillConfirmed.AddDynamic(this, &APlayerCharacter::OnSkillConfirmed);
	}

	if (CachedAttributeComponent)
	{
		CachedAttributeComponent->OnBPUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::UpdatePlayerBP);
		CachedAttributeComponent->OnHPUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::UpdatePlayerHP);
		//For Level Text Update
		CachedAttributeComponent->OnLevelUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::UpdateLevelText);
		//For Level Up PopUp 
		CachedAttributeComponent->OnLevelUp.AddDynamic(this, &APlayerCharacter::OnLevelUp);
		CachedAttributeComponent->OnDamageCalculated.AddDynamic(this, &APlayerCharacter::OnDamageCalculated);
		CachedAttributeComponent->OnBPMax.AddDynamic(this, &APlayerCharacter::SetIsSkillEnable);
		CachedAttributeComponent->OnSkillTreeUISet.AddDynamic(this, &APlayerCharacter::OnSkillTreeSet);
		CachedAttributeComponent->OnSkillPointUpdate.AddDynamic(CachedHUDComponent, &UHUDComponent::SetSkillPointText);
	}

	if (ARPGGameMode* GM = Cast<ARPGGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GM->OnEnterSaloon.AddDynamic(this, &APlayerCharacter::EnterSaloonMode);
		GM->OnExitSaloon.AddDynamic(this, &APlayerCharacter::ExitSaloonMode);
		GM->OnDialogueEvent.AddDynamic(CachedHUDComponent, &UHUDComponent::ShowDialogue);

		//Save When move to another level
		GM->OnSaveRequired.AddDynamic(CachedAttributeComponent, &UPlayerAttributeComponent::SavePlayerData);

		//load when open a new map

	}

	SetupPlayerInputComponent(InputComponent);

	//For Debug
	//CachedHUDComponent->ShowSkillTreeUI();

	//CachedUltimateSkillComponent->SetCachedComboPatters(CachedComboManageComponent->GetComboPatterns());



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
	//play Combo Anim
	const EAnimMontageType ComboMontage = static_cast<EAnimMontageType>(ComboNum);
	CachedAnimInstance->PlayComboAttackMontage(ComboMontage);
	CachedSFXComponent->PlayComboAttackSFX(ComboNum);
	
	//PlayAnimMontage(ComboAnimNum); //Play Combo Animation
	CachedAttributeComponent->SetBPtoGet(BPValue);//Set BPValue to get
	UE_LOG(LogTemp, Warning, TEXT("Combo!!"));

	//Combo PopUp
	CachedHUDComponent->ShowComboPopUp(ComboNum);

	//감염 스킬 On 상태에서 콤보 Match 시 적으로 Broadcast
	//For SkillC
	if (CachedUltimateSkillComponent->IsSkillActivated() && SelectedSkill == EUltimateSkill::SkillC)
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
	if (CachedUltimateSkillComponent->IsSkillActivated() && SelectedSkill == EUltimateSkill::SkillB)
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

//called when start game
void APlayerCharacter::OnSkillTreeSet(int32 SkillPoint, int32 UltNum, TArray<int32> ActivatedSkills, int32 Ult1XSetting, int32 Ult1YSetting)
{
	SetUltimateSkill(UltNum);
	CachedHUDComponent->SetSkillTree(SkillPoint, UltNum, ActivatedSkills, Ult1XSetting, Ult1YSetting);
}

void APlayerCharacter::OnQTEFailed()
{
	UE_LOG(LogTemp, Error, TEXT("QTE FAILED!!!!!!"));
	CachedHUDComponent->HideQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());
	CachedAnimInstance->PlayAnimMontage(3);
}

//Callback Func (from delegate at UltSkillComp)
void APlayerCharacter::OnSkill1Attack(int32 AnimNum, float AttackMultiplier)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Broadcast Received"));
	CachedAttributeComponent->SetAttackMultiplier(AttackMultiplier);
	//play Combo Anim
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

	//HUD 함수 아래에 연결
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

	if (CachedUltimateSkillComponent->IsSkillActivated() && SelectedSkill == EUltimateSkill::SkillA)
		//if (SelectedSkill == EUltimateSkill::SkillA)
	{
		CachedUltimateSkillComponent->Skill1(AttackValue);
	}

	EAttackKeyType InputType;

	if (GetPlayerCombatState() == EPlayerCombatState::EPCS_NotAttacking)
	{
		SetPlayerCombatState(EPlayerCombatState::EPCS_Attacking);
		//disable move input when player is attacking
		SetPlayerMoveState(EPlayerMoveState::EPMS_MoveDisabled);

		//X Button
		if (AttackValue == 1.f)
		{
			InputType = EAttackKeyType::X;
		}
		//Y Button
		else if (AttackValue == -1.f)
		{
			InputType = EAttackKeyType::Y;
		}

		//When Player Is Trapped by Boss's Ult Skill -> RandomComboQTE Start!
		if (IsTrapped)
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
				//Trapped 상태 풀림
				IsTrapped = false;

				CachedHUDComponent->HideQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());
				CachedHUDComponent->NoticeToBreak();

			}
			return;
		}

		//send ENUM to InputSequence array (for Combo check)
		if (!CachedComboManageComponent->IsComboMatch(InputType))
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
		CachedAttributeComponent->SetAttackMultiplier(CachedComboManageComponent->GetCurrentAttackMultiplier());
	}
}


void APlayerCharacter::SkillStart()
{
	//for debug
	IsSkillEnable = true;

	if (IsSkillEnable)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillStart!!!"));

		//SKill 1
		if (SelectedSkill == EUltimateSkill::SkillA)
		{
			CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
			CachedUltimateSkillComponent->SetCachedComboPatters(CachedComboManageComponent->GetComboPatterns());
			UE_LOG(LogTemp, Display, TEXT("SkillA"));
		}
		else if (SelectedSkill == EUltimateSkill::SkillB)
		{
			CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
			UE_LOG(LogTemp, Display, TEXT("SkillB"));
		}
		else if (SelectedSkill == EUltimateSkill::SkillC)
		{
			CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
			UE_LOG(LogTemp, Display, TEXT("SkillC"));
		}

		const int32 SkillNum = static_cast<int32>(SelectedSkill);

		CachedHUDComponent->ShowSkillEffect(SkillNum);
		CachedSFXComponent->PlayUltSkillSFX(SkillNum);

		//Make BP to Zero (0) 
		UpdateBP(0, CachedAttributeComponent->GetMaxBP());

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
		SetUltimateSkill(Index);
		break;

	case ESkillConfirmType::NormalSkill:
		if (CachedAttributeComponent->GetSkillPoint() > 0) //Get Skill Only when SkillPoint Remains
		{
			CachedAttributeComponent->UpdateStatsBySkill(Index);
		}
		break;

	case ESkillConfirmType::SkillTimeUp:
		CachedUltimateSkillComponent->IncreaseSkillTime();
		break;

	case ESkillConfirmType::KeyBindingX:
		CachedUltimateSkillComponent->SetKeyBindingXInSkillA(Index);
		break;

	case ESkillConfirmType::KeyBindingY:
		CachedUltimateSkillComponent->SetKeyBindingYInSkillA(Index);
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

void APlayerCharacter::EnterSaloonMode()
{
	//IsInSaloon = true;
	UE_LOG(LogTemp, Error, TEXT("Set IsInSaloon TRUE"));
}

void APlayerCharacter::ExitSaloonMode()
{
	//IsInSaloon = false;
}

void APlayerCharacter::OnInteract()
{
	CachedInteractionComponent->InteractionStart();
}

void APlayerCharacter::SetIsSkillEnable()
{
	UE_LOG(LogTemp, Warning, TEXT("SkillEnable !!BPMAX!!"));
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
			UE_LOG(LogTemp, Warning, TEXT("LowHP!!!"));
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


