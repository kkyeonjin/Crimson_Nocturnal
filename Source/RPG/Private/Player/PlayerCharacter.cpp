// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimMontage.h"
#include "Player/Animation/PlayerAnimInstance.h"
#include "Player/PlayerComponents/ComboManageComponent.h"
#include "Player/PlayerComponents/HUDComponent.h"
#include "Player/PlayerComponents/WeaponComponent.h"
#include "Player/PlayerComponents/KickComponent.h"
#include "Player/PlayerComponents/PlayerAttributeComponent.h"
#include "Player/PlayerComponents/UltimateSkillComponent.h"


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
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
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
		CachedAnimInstance->OnKickCollisionEnable.AddDynamic(this, &APlayerCharacter::SetKickCollisionDisable);
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
	if (!CachedAttributeComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerAttributeComp Cannot Find"));
	}

	CachedUltimateSkillComponent = FindComponentByClass<UUltimateSkillComponent>();
	if (CachedUltimateSkillComponent)
	{
		CachedUltimateSkillComponent->OnSkill1Attack.AddDynamic(this, &APlayerCharacter::OnSkill1Attack);
	}

	CachedHUDComponent = FindComponentByClass<UHUDComponent>();
	CachedHUDComponent->UpdatePlayerHP(
		CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP()
	);
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

void APlayerCharacter::OnSkill1Attack(int32 AnimNum, float AttackMultiplier)
{
	UE_LOG(LogTemp, Warning, TEXT("Skill Broadcast Received"));
	CachedAttributeComponent->SetAttackMultiplier(AttackMultiplier);
	CachedAnimInstance->PlayAnimMontage(AnimNum);
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
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

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	const float AttackValue = Value.Get<float>();

	if(CachedUltimateSkillComponent->IsSkillActivated() && SelectedSkill==EUltimateSkill::SkillA)
	{
		CachedUltimateSkillComponent->Skill1(AttackValue);
	}

	else
	{
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
	//SKill 1
	CachedUltimateSkillComponent->TimerStart(&SelectedSkill);
	CachedUltimateSkillComponent->SetCachedComboPatters(CachedComboManageComponent->GetComboPatterns());

}

void APlayerCharacter::SkillTree()
{
	CachedHUDComponent->ShowSkillTreeUI();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

//automatically called by UnrealEngine 
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(SkillActivation, ETriggerEvent::Triggered, this, &APlayerCharacter::SkillStart);
		EnhancedInputComponent->BindAction(SkillTreeOn, ETriggerEvent::Triggered, this, &APlayerCharacter::SkillTree);
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
	CachedAttributeComponent->SetEnemyAttributeComponent(Target);
	CachedAttributeComponent->ApplyDamage();
	CachedHUDComponent->UpdatePlayerHP(
		CachedAttributeComponent->GetCurrentHP(), CachedAttributeComponent->GetMaxHP()
	);
}








