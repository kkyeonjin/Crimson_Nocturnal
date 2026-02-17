#pragma once

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	EECS_Attacking UMETA(DisplayName = "Attacking"),
	EECS_NotAttacking UMETA(DisplayName = "NotAttacking"),
	EECS_GetAttacked UMETA(DisplayName = "GetAttacked"),
	EECS_Dead UMETA(DisplayName = "Dead")

};

namespace EnemyConstants
{
	constexpr float PATROL_SPEED = 250.0f;
	constexpr float CHASE_SPEED = 500.0f;
	constexpr float DEATH_ANIMATION_DURATION = 2.f;
	constexpr float FADEOUT_DURATION = 2.f;
}
