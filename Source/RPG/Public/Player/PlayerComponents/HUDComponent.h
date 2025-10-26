// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDComponent.generated.h"

class USkillTreeWidget;
class UMainHUDWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UHUDComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHUDComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;  // 블루프린트 클래스를 지정

	UPROPERTY()
	UMainHUDWidget* MainHUDWidget;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USkillTreeWidget> SkillTreeUIClass;  // 블루프린트 클래스를 지정

	UPROPERTY()
	USkillTreeWidget* SkillTreeUIWidget;

	void UpdatePlayerHP(int32 Current, int32 Max);

	void ShowSkillTreeUI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

		
};
