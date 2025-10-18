#pragma once

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPCS_Attacking UMETA(DisplayName = "Attacking"),
	EPCS_NotAttacking UMETA(DisplayName = "NotAttacking"),
	EPCS_GetAttacked UMETA(DisplayName = "GetAttacked")

};

//접두사 안 붙이는 게 나음 -> 추후 다른 ENUM들도 변경 필 
UENUM(BlueprintType)
enum class EUltimateSkill : uint8
{
	None,
	SkillA,
	SkillB,
	SkillC
};

UENUM(BlueprintType)
enum class EPlayerMoveState : uint8
{
	EPMS_MoveEnabled UMETA(DisplayName = "MoveEnabled"),
	EPMS_MoveDisabled UMETA(DisplayName = "MoveDisabled")

};

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	EPM_TravelMode UMETA(DisplayName = "TravelMode"),
	EPM_BattleMode UMETA(DisplayName = "BattleMode")
};
