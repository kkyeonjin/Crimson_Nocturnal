// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/Boss/Types/BossTypes.h"
#include "Components/ActorComponent.h"
#include "PlayerEventHandlerComponent.generated.h"

class APlayerCharacter;
class UComboManageComponent;
class UPlayerAttributeComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UPlayerEventHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

    /*
    UPlayerEventHandlerComponent();

    virtual void BeginPlay() override;

    // 모든 콜백 함수들이 여기 모임
    UFUNCTION()
    void OnComboComplete(int32 ComboAnimNum, int32 BPValue);

    UFUNCTION()
    void OnSkillConfirmed(int32 SkillNum);

    UFUNCTION()
    void OnDamageReceived(int32 Damage, AActor* Attacker);

    UFUNCTION()
    void OnPlayerLevelUp(int32 NewLevel);

    UFUNCTION()
    void OnBossPhaseChanged(EBossPhase NewPhase);

    // ... 나머지 콜백들

private:
    UPROPERTY()
    APlayerCharacter* OwnerPlayer;

    // 필요한 컴포넌트 참조들만
    UPROPERTY()
    UComboManageComponent* CachedComboComponent;

    UPROPERTY()
    UPlayerAttributeComponent* CachedAttributeComponent;

    // ... 필요한 것들만

    void BindDelegates();
	*/
		
};
