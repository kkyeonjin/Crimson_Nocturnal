#pragma once

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPCS_Attacking UMETA(DisplayName = "Attacking"),
	EPCS_NotAttacking UMETA(DisplayName = "NotAttacking")

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
