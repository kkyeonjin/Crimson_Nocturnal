// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Character/Player/PlayerAnimInstance.h"
#include "Component/Player/ComboManageComponent.h"
#include "Component/Player/HUDComponent.h"
#include "Component/Player/WeaponComponent.h"
#include "Component/Player/KickComponent.h"
#include "Component/Player/PlayerAttributeComponent.h"
#include "Component/Player/UltimateSkillComponent.h"
#include "Component/Player/RaycastComponent.h"
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
		InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		if (InputSubsystem)
		{
			InputSubsystem->AddMappingContext(PlayerContext, 0);
		}
	}
	//cast animInstance In here (for reuse)
	CachedAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
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
	}

	CachedComboManageComponent = FindComponentByClass<UComboManageComponent>();
	if (CachedComboManageComponent)
	{
		CachedComboManageComponent->OnComboMatch.AddDynamic(this, &APlayerCharacter::OnComboComplete);
		CachedComboManageComponent->OnChainAttack.AddDynamic(this, &APlayerCharacter::OnChainAttack);
	}

	CachedWeaponComponent = FindComponentByClass<UWeaponComponent>();
	CachedKickComponent = FindComponentByClass<UKickComponent>();

	CachedAttributeComponent = FindComponentByClass<UPlayerAttributeComponent>();
	if (CachedAttributeComponent)
	{
		CachedAttributeComponent->OnBPUpdate.AddDynamic(this, &APlayerCharacter::UpdateBP);
		CachedAttributeComponent->OnHPUpdate.AddDynamic(this, &APlayerCharacter::UpdateHP);
		CachedAttributeComponent->OnLevelUp.AddDynamic(this, &APlayerCharacter::OnLevelUp);
		CachedAttributeComponent->OnDamageCalculated.AddDynamic(this, &APlayerCharacter::OnDamageCalculated);
	}

	CachedUltimateSkillComponent = FindComponentByClass<UUltimateSkillComponent>();
	if (CachedUltimateSkillComponent)
	{
		CachedUltimateSkillComponent->OnSkill1Attack.AddDynamic(this, &APlayerCharacter::OnSkill1Attack);
	}

	CachedHUDComponent = FindComponentByClass<UHUDComponent>();
	if (CachedHUDComponent)
	{
		//Initialize HP&BP Bar
		UpdateHP(CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP());
		UpdateBP(CachedAttributeComponent->GetCurrentBP(), CachedAttributeComponent->GetMaxBP());

		CachedHUDComponent->OnPlayerSkillConfirmed.AddDynamic(this, &APlayerCharacter::OnSkillConfirmed);
	}

	CachedRaycastComponent = FindComponentByClass<URaycastComponent>();
	if (CachedRaycastComponent)
	{
		
	}
}

void APlayerCharacter::OnAttackMontageEnded()
{
	SetPlayerCombatState(EPlayerCombatState::EPCS_NotAttacking);
	CachedWeaponComponent->InitializeActorsToIgnore();
	CachedKickComponent->InitializeActorsToIgnore();
}

//delegate callback function (when combo matches)
void APlayerCharacter::OnComboComplete(int32 ComboAnimNum, int32 BPValue)
{
	CachedAnimInstance->PlayAnimMontage(ComboAnimNum); //Play Combo Animation
	CachedAttributeComponent->SetBPtoGet(BPValue);//Set BPValue to get
	UE_LOG(LogTemp, Warning, TEXT("Combo!!"));

	//감염 스킬 On 상태에서 콤보 Match 시 적으로 Broadcast
	if (CachedUltimateSkillComponent->IsSkillActivated() && SelectedSkill == EUltimateSkill::SkillC)
	{
		OnSkillCReady.Broadcast();
	}
}

void APlayerCharacter::OnChainAttack(int32 AnimNum, FName SectionName)
{
	CachedAnimInstance->PlayChainAttackMontage(AnimNum, SectionName);
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

void APlayerCharacter::OnLevelUp(int32 SkillPoint)
{
	UpdateHP(CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP());
	CachedHUDComponent->SetSkillPointText(SkillPoint);
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

//Callback Func (from delegate at UltSkillComp)
void APlayerCharacter::OnSkill1Attack(int32 AnimNum, float AttackMultiplier)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Broadcast Received"));
	CachedAttributeComponent->SetAttackMultiplier(AttackMultiplier);
	CachedAnimInstance->PlayAnimMontage(AnimNum);
}

void APlayerCharacter::StartBossQTEMode()
{
	IsTrapped = true;
	//Set Random QTE Combo Num
	CachedComboManageComponent->SetRequiredBossComboNum();

	//HUD 함수 아래에 연결
	CachedHUDComponent->ShowQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//Disable Move Func When Player Is Trapped by Boss's Ult Skill
	if (IsTrapped)
	{
		return;
	}

	//if (GetPlayerMoveState() == EPlayerMoveState::EPMS_MoveEnabled)
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
	//For Debug
	if (IsTrapped)
	{
		UE_LOG(LogTemp, Display, TEXT("QTE Attack Button Pressed"));

	}

	const float AttackValue = Value.Get<float>();

	if (IsSkillTreeUIOn) //InSkillTreeUI (Set SkillA Combo Key Setting)
	{
		//X -> 1 / Y -> -1
		CachedHUDComponent->ReadyToSkillASetting(static_cast<int32>(AttackValue));
		/*
		//X Button
		if (AttackValue == 1.f)
		{
		CachedHUDComponent->ReadyToSkillASetting(static_cast<int32>(AttackValue));
		UE_LOG(LogTemp, Display, TEXT("X Setting"));
		}
		//Y Button
		else if (AttackValue == -1.f)
		{
		UE_LOG(LogTemp, Display, TEXT("Y Setting"));
		}*/
	}

	else
	{
		if (CachedUltimateSkillComponent->IsSkillActivated() && SelectedSkill == EUltimateSkill::SkillA)
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
				UE_LOG(LogTemp, Display, TEXT("Input Type: %d"), static_cast<int32>(InputType));
			}
			//Y Button
			else if (AttackValue == -1.f)
			{
				InputType = EAttackKeyType::Y;
				UE_LOG(LogTemp, Display, TEXT("Input Type: %d"), static_cast<int32>(InputType));
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

					UE_LOG(LogTemp, Warning, TEXT("QTE Failed!!!"));

				}
				else
				{


					//Broadcast to Boss
					OnQTESuccess.Broadcast();
					//Trapped 상태 풀림
					IsTrapped = false;

					CachedHUDComponent->HideQTEUI(CachedComboManageComponent->GetRequiredBossComboNum());

					//Player Die
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
}

void APlayerCharacter::SkillStart()
{
	UE_LOG(LogTemp, Warning, TEXT("SkillStart!!!"));

	//SKill 1
	if (SelectedSkill == EUltimateSkill::SkillA)
	{
		CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
		CachedUltimateSkillComponent->SetCachedComboPatters(CachedComboManageComponent->GetComboPatterns());
	}
	else if (SelectedSkill == EUltimateSkill::SkillB)
	{
		CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
	}
	else if (SelectedSkill == EUltimateSkill::SkillC)
	{
		CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
	}

}

//press skillTree button
void APlayerCharacter::SkillTree()
{
	CachedHUDComponent->ShowSkillTreeUI();
	CachedHUDComponent->InitializeSkillTreeSelection();
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		//PC->SetPause(true); // 게임 정햨E

		// 게임 + UI 입력 모두 활성화 (게임패탛E입력 포함)
		//PC->SetInputMode(FInputModeGameAndUI());

		//CachedHUDComponent->GetSkillTreeUI()->SetUserFocus(PC);
	}

	InputSubsystem->RemoveMappingContext(PlayerContext);
	InputSubsystem->AddMappingContext(SkillTreeContext, 1);
	IsSkillTreeUIOn = true;
	//CachedHUDComponent->GetSkillTreeUI()->SetUserFocus(PC);
}

//SkillTreeInputContext
//Cross Button in GamePad
void APlayerCharacter::Select(const FInputActionValue& Value)
{
	const FVector2D SelectVector = Value.Get<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("SelectVector X=%f, Y=%f"), SelectVector.X, SelectVector.Y);

	if (SelectVector.X != 0.f) //Left&Right
	{
		//Section Change or SkillA Option Navigate
		CachedHUDComponent->HandleSkillTreeNavigation(static_cast<int32>(SelectVector.X));

	}

	if (SelectVector.Y != 0.f) //Select Up&Down
	{
		CachedHUDComponent->ChangeSelection(static_cast<int32>(SelectVector.Y));
	}
}


void APlayerCharacter::SkillTreeOff()
{
	UE_LOG(LogTemp, Display, TEXT("SkillTreeOffButton"));
	CachedHUDComponent->HideSkillTreeUI();
	GetWorld()->GetFirstPlayerController()->SetPause(false); //Start Game Play
	InputSubsystem->RemoveMappingContext(SkillTreeContext);
	InputSubsystem->AddMappingContext(PlayerContext, 0);

	IsSkillTreeUIOn = false;
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

//SkillTreeInputContext
//A Button
void APlayerCharacter::SetSkillTree() //Renaming Required
{
	CachedHUDComponent->SetSkillTree();
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
		EnhancedInputComponent->BindAction(SkillTreeOn, ETriggerEvent::Triggered, this, &APlayerCharacter::SkillTree);

		//SkillTreeInputContext
		EnhancedInputComponent->BindAction(SkillTreeOffAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SkillTreeOff);
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Select);
		EnhancedInputComponent->BindAction(SkillEquipAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SetSkillTree);
	}
}

void APlayerCharacter::HandleAttack(AActor* Target)
{
	CachedAttributeComponent->SetEnemyAttributeComponent(Target);
}

void APlayerCharacter::GetHit(const FVector& ImpactPoint, AActor* Target)
{
	UE_LOG(LogTemp, Error, TEXT("Player Hit!!!"));
	CachedAnimInstance->PlayAnimMontage(2);
	PlayerMoveState = EPlayerMoveState::EPMS_MoveDisabled;
	//CachedAttributeComponent->SetEnemyAttributeComponent(Target);
	//CachedAttributeComponent->ApplyDamage();
}


