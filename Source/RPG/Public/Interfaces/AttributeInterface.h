// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAttributeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IAttributeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetATK() = 0;
	virtual float GetDEF() = 0;

	virtual float CalcDamage() = 0;
	virtual void ApplyDamage() = 0;

};
