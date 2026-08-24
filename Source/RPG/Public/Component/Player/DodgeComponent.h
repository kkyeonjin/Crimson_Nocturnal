// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DodgeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UDodgeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDodgeComponent();

protected:
	virtual void BeginPlay() override;

		
};
