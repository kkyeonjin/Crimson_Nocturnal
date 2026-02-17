#pragma once

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPCS_Attacking UMETA(DisplayName = "Attacking"),
	EPCS_NotAttacking UMETA(DisplayName = "NotAttacking"),
	EPCS_GetAttacked UMETA(DisplayName = "GetAttacked"),
	EPCS_Dead UMETA(DisplayName = "Dead")
};


UENUM(BlueprintType)
enum class EUltimateSkill : uint8
{
	SkillA = 0,
	SkillB = 1,
	SkillC = 2,
	None = 10
};

UENUM(BlueprintType)
enum class EPlayerMoveState : uint8
{
	EPMS_MoveEnabled UMETA(DisplayName = "MoveEnabled"),
	EPMS_MoveDisabled UMETA(DisplayName = "MoveDisabled")

};
