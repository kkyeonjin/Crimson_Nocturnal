// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EBossPhase : uint8
{
	Phase1_MagicCircle = 1 UMETA(DisplayName = "Magic Circle"),
	Phase2_WingAttack = 2 UMETA(DisplayName = "Wing Attack"),
	Phase3_GroundSlam = 3 UMETA(DisplayName = "Ground Slam"),
	Ultimate = 4 UMETA(DisplayName = "Ultimate"),
	Defeated = 0 UMETA(DisplayName = "Defeated")
};


// ? Boss »ó¼ö
namespace BossConstants
{
	constexpr float PHASE1_DURATION = 7.5f;
	constexpr float PHASE2_DURATION = 4.5f;
	constexpr float PHASE3_DURATION = 6.f;
	constexpr float ROAR_TIMER = 1.5f;
}

