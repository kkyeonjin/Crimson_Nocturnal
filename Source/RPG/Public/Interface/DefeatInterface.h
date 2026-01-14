// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DefeatInterface.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDefeated, class AActor*, DefeatedCharacter);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDefeatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RPG_API IDefeatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// "이 인터페이스를 구현한 클래스는 FOnCharacterDead 델리게이트를 가져야 한다"는 계약
	// 이 함수를 통해서만 그 델리게이트를 빌려줄 수 있음
	virtual FOnCharacterDefeated& OnCharacterDefeatedDelegate() = 0;

};
