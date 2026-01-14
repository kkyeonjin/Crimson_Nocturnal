// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1_MagicCircle UMETA(DisplayName = "Magic Circle"),
	Phase2_WingAttack UMETA(DisplayName = "Wing Attack"),
	Phase3_GroundSlam UMETA(DisplayName = "Ground Slam"),
	Ultimate UMETA(DisplayName = "Ultimate"),
	Defeated UMETA(DisplayName = "Defeated")
};


// ? Boss »ó¼ö
namespace BossConstants
{
	constexpr float PHASE1_DURATION = 4.0f;
	constexpr float PHASE2_DURATION = 3.5f;
	constexpr float PHASE3_DURATION = 4.5f;
	constexpr float ROAR_TIMER = 1.5f;
}

