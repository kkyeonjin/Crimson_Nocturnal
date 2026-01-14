// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Base/BaseAttributeComponent.h"

UBaseAttributeComponent::UBaseAttributeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UBaseAttributeComponent::BeginPlay()
{
    Super::BeginPlay();

}

int32 UBaseAttributeComponent::CalcDamage(int32 AttackerATK, int32 DefenderDEF)
{
    // 기본 데미지 계산
    int32 BaseDamage = AttackerATK - DefenderDEF;

    // 최소 데미지 적용
    int32 FinalDamage = FMath::Max(BaseDamage, MinDamage);

    return FinalDamage;
}

void UBaseAttributeComponent::ApplyDamage(int32 Damage, AActor* Attacker)
{
    // 이미 죽었으면 무시
    if (IsDead())
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Already dead, cannot apply damage"), *GetOwner()->GetName());
        return;
    }

    // 데미지가 0 이하면 무시
    if (Damage <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Invalid damage: %d"), *GetOwner()->GetName(), Damage);
        return;
    }

    // HP 감소
    CurrentHP = FMath::Max(CurrentHP - Damage, 0);

    // OnDamageReceived Broadcast
    OnDamageReceived.Broadcast(Damage, Attacker);

    // HP 변화 알림
    NotifyHPChanged();

    // 죽음 처리
    if (IsDead())
    {
        OnDeath();
    }

    UE_LOG(LogTemp, Display, TEXT("[%s] Took %d damage. HP: %d/%d"),
        *GetOwner()->GetName(), Damage, CurrentHP, MaxHP);
}

void UBaseAttributeComponent::Heal(int32 HealAmount)
{
    if (IsDead())
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Dead, cannot heal"), *GetOwner()->GetName());
        return;
    }

    if (HealAmount <= 0)
    {
        return;
    }

    int32 OldHP = CurrentHP;
    CurrentHP = FMath::Min(CurrentHP + HealAmount, MaxHP);

    int32 ActualHeal = CurrentHP - OldHP;

    NotifyHPChanged();

    UE_LOG(LogTemp, Display, TEXT("[%s] Healed %d. HP: %d/%d"),
        *GetOwner()->GetName(), ActualHeal, CurrentHP, MaxHP);
}

void UBaseAttributeComponent::SetMaxHP(int32 NewMaxHP)
{
    if (NewMaxHP <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("[%s] Invalid MaxHP: %d"), *GetOwner()->GetName(), NewMaxHP);
        return;
    }

    MaxHP = NewMaxHP;
    CurrentHP = FMath::Min(CurrentHP, MaxHP);

    NotifyHPChanged();
}

void UBaseAttributeComponent::OnDeath()
{
    OnDead.Broadcast();

    UE_LOG(LogTemp, Warning, TEXT("[%s] is dead!"), *GetOwner()->GetName());
}

void UBaseAttributeComponent::NotifyHPChanged()
{
    if (bBroadcastHPChanges)
    {
        OnHPChanged.Broadcast(CurrentHP, MaxHP);
    }
}

// 인터페이스 호환용 (파라미터 없는 버전)
int32 UBaseAttributeComponent::CalcDamage()
{
    // 기본 구현: 자기 ATK - 자기 DEF (의미 없음, 상속 클래스에서 오버라이드)
    return FMath::Max(ATK - DEF, MinDamage);
}

void UBaseAttributeComponent::ApplyDamage()
{
    // 기본 구현: 자기 ATK로 자기 데미지 (테스트용)
    int32 SelfDamage = CalcDamage(ATK, DEF);
    ApplyDamage(SelfDamage, nullptr);
}
