#pragma once

UENUM(BlueprintType)
enum class EEnemyCombatState : uint8
{
	EECS_Attacking UMETA(DisplayName = "Attacking"),
	EECS_NotAttacking UMETA(DisplayName = "NotAttacking"),
	EECS_GetAttacked UMETA(DisplayName = "GetAttacked"),
	EECS_Dead UMETA(DisplayName = "Dead")

};