// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AttributeInterface.h"
#include "BaseAttributeComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDamageReceived, int32, Damage, AActor*, Attacker);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHPChanged, int32, CurrentHP, int32, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

/**
 * 모든 캐릭터 (Player, Enemy, Boss) 속성의 Base Class
 *
 * 책임:
 * - HP/MaxHP/ATK/DEF 관리
 * - 데미지 계산 및 적용
 * - HP 변화 알림
 *
 * 상속 클래스:
 * - UPlayerAttributeComponent (Player 전용 추가: BP)
 * - UEnemyAttributeComponent (Enemy 전용)
 * - UBossAttributeComponent (Boss 전용)
 */

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class RPG_API UBaseAttributeComponent : public UActorComponent, public IAttributeInterface
{
	GENERATED_BODY()

public:
    UBaseAttributeComponent();
    virtual void BeginPlay() override;

    // ==================== 데미지 시스템 ====================
     /**
      * 데미지 계산
      *
      * 로직:
      * BaseDamage = 공격자 ATK - 방어자 DEF
      * FinalDamage = Max(BaseDamage, MinDamage)
      *
      * @return 최종 데미지 (0 이상)
      */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual int32 CalcDamage(int32 AttackerATK, int32 DefenderDEF);

    /**
     * 데미지 적용
     *
     * 로직:
     * 1. CurrentHP 감소
     * 2. HP 범위 체크 (0 ~ MaxHP)
     * 3. OnDamageReceived Broadcast
     * 4. OnHPChanged Broadcast
     *
     * @param Damage 적용할 데미지
     * @param Attacker 공격자 (nullptr 가능)
     */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void ApplyDamage(int32 Damage, AActor* Attacker = nullptr);

    /**
     * 치유
     *
     * @param HealAmount 치유량
     */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void Heal(int32 HealAmount);

    // ==================== Getter ====================
    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual int32 GetATK() const { return ATK; }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual int32 GetDEF() const { return DEF; }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual int32 GetCurrentHP() const { return CurrentHP; }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual int32 GetMaxHP() const { return MaxHP; }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual bool IsDead() const { return CurrentHP <= 0; }

    // ==================== Setter (상속 클래스에서 필요시 사용) ====================
    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void SetMaxHP(int32 NewMaxHP);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void SetATK(int32 NewATK) { ATK = FMath::Max(NewATK, 0); }

    UFUNCTION(BlueprintCallable, Category = "Stats")
    virtual void SetDEF(int32 NewDEF) { DEF = FMath::Max(NewDEF, 0); }

    // ==================== Delegate ====================
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnHPChanged OnHPChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDamageReceived OnDamageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnDead OnDead;

    // ==================== IAttributeInterface 구현 (인터페이스 전용) ====================
    virtual int32 GetATK() override { return ATK; }
    virtual int32 GetDEF() override { return DEF; }
    virtual int32 GetCurrentHP() override { return CurrentHP; }
    virtual int32 GetMaxHP() override { return MaxHP; }

    virtual int32 CalcDamage() override;
    virtual void ApplyDamage() override;

protected:
    // ==================== 속성 ====================
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "1"))
    int32 MaxHP = 100;

    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 CurrentHP = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0"))
    int32 ATK = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (ClampMin = "0"))
    int32 DEF = 5;

    // ==================== 설정 ====================
    // 최소 데미지 (0 미만 방지)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    int32 MinDamage = 0;

    // HP 변화 알림 제한 (성능)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
    bool bBroadcastHPChanges = true;

    // ==================== 내부 함수 ====================
    /**
     * 죽음 처리 (상속 클래스에서 override 가능)
     */
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual void OnDeath();

    /**
     * HP 변화 알림
     */
    void NotifyHPChanged();
};
