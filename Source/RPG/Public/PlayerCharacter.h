// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerType.h" //for EPlayerState
#include "ComboInput.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class AItem;
class UAnimMontage;
class UPlayerAnimInstance;
class UComboManageComponent;

UCLASS()
class RPG_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	virtual void Jump() override;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;

	//Function for Input Binding 
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);

	UFUNCTION()
	void OnAttackMontageEnded();

	UFUNCTION()
	void OnComboComplete(int32 ComboNum);

private:
	//default state is unequipped
	EPlayerState PlayerState = EPlayerState::ECS_UnEquipped;

	UPlayerAnimInstance* CachedAnimInstance;

	UComboManageComponent* CachedComboManageComponent;

	//only visible in Detail Panel
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE EPlayerState GetPlayerState() const { return PlayerState; }
	FORCEINLINE void SetPlayerState(EPlayerState State) { PlayerState = State; }


};
