// Manage animations montage playing of player

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Character/Player/Types/ComboInput.h"
#include "PlayerAnimInstance.generated.h"

class UCharacterMovementComponent;
class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackMontageEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoveEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionEnable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollisionDisable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvincibleStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInvincibleEnd);

UCLASS()
class RPG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAnimMontage(int32 AnimNum);

	void PlayChainAttackMontage(int32 AnimNum, FName Section);

	void PlayComboAttackMontage(EAnimMontageType MontageType);

	UPROPERTY(BlueprintAssignable)
	FOnAttackMontageEnd OnAttackMontageEnd;

	UPROPERTY(BlueprintAssignable)
	FOnMoveEnable OnMoveEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionEnable OnBladeLCollisionEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionDisable OnBladeLCollisionDisable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionEnable OnBladeRCollisionEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionDisable OnBladeRCollisionDisable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionEnable OnKickCollisionEnable;

	UPROPERTY(BlueprintAssignable)
	FOnCollisionDisable OnKickCollisionDisable;

	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

	UPROPERTY(BlueprintAssignable)
	FOnInvincibleStart OnInvincibleStart;

	UPROPERTY(BlueprintAssignable)
	FOnInvincibleEnd OnInvincibleEnd;


protected:
	// Callback funcs by animnotify
	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_MoveEnable();

	UFUNCTION()
	void AnimNotify_BladeLCollisionEnable();

	UFUNCTION()
	void AnimNotify_BladeLCollisionDisable();

	UFUNCTION()
	void AnimNotify_BladeRCollisionEnable();

	UFUNCTION()
	void AnimNotify_BladeRCollisionDisable();

	UFUNCTION()
	void AnimNotify_KickCollisionEnable();

	UFUNCTION()
	void AnimNotify_KickCollisionDisable();

	UFUNCTION()
	void AnimNotify_GameOver();

	UFUNCTION()
	void AnimNotify_InvincibleStart();

	UFUNCTION()
	void AnimNotify_InvincibleEnd();

	UPROPERTY(BlueprintReadWrite)
	float GroundSpeed;
	UPROPERTY(BlueprintReadWrite)
	bool IsFalling;


private:
	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY()
	UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackA;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackB;

	UPROPERTY(EditAnywhere, Category = "Combo")
	UAnimMontage* ComboA;

	UPROPERTY(EditAnywhere, Category = "Combo")
	UAnimMontage* ComboB;

	UPROPERTY(EditAnywhere, Category = "Combo")
	UAnimMontage* ComboC;

	UPROPERTY(EditAnywhere, Category = "Combo")
	UAnimMontage* ComboD;

	UPROPERTY(EditAnywhere, Category = "Stun")
	UAnimMontage* StunA;

	UPROPERTY(EditAnywhere, Category = "Death")
	UAnimMontage* Death;

	UPROPERTY(EditAnywhere, Category = "Dance")
	UAnimMontage* Dance;

	UPROPERTY(EditAnywhere, Category = "Dodge")
	UAnimMontage* Dodge;
public:
	FORCEINLINE float GetGroundSpeed() const { return GroundSpeed; }

};
