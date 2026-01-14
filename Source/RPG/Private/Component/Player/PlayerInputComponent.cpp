// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Player/PlayerInputComponent.h"
#include "Character/Player/PlayerCharacter.h"
#include "Component/Player/ComboManageComponent.h"
#include "Component/Player/PlayerAttributeComponent.h"

// Sets default values for this component's properties
UPlayerInputComponent::UPlayerInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

/*

// Called when the game starts
void UPlayerInputComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerCharacter>(GetOwner());
	if (!OwnerPlayer) return;

	// 필요한 컴포넌트만 찾고 Bind
	CachedComboComponent = OwnerPlayer->FindComponentByClass<UComboManageComponent>();
	CachedAttributeComponent = OwnerPlayer->FindComponentByClass<UPlayerAttributeComponent>();

	BindDelegates();
	
}

void UPlayerInputComponent::BindDelegates()
{
    if (CachedComboComponent)
    {
        CachedComboComponent->OnComboMatch.AddDynamic(this, &UPlayerInputComponent::OnComboComplete);
    }

    if (CachedAttributeComponent)
    {
        //CachedAttributeComponent->OnDamageReceived.AddDynamic(this, &UPlayerInputComponent::OnDamageReceived);
    }

    // ... 나머지
}

void UPlayerInputComponent::OnComboComplete(int32 ComboAnimNum, int32 BPValue)
{
    // 콤보 완료 로직
    if (OwnerPlayer)
    {
        // 필요한 처리들...
    }
}


*/