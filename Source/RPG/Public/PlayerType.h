#pragma once

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped UMETA(DisplayName = "Equipped"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_NotAttacking UMETA(DisplayName = "NotAttacking")

};

UENUM(BlueprintType)
enum class EPlayerMode : uint8
{
	EPM_TravelMode UMETA(DisplayName = "TravelMode"),
	EPM_BattleMode UMETA(DisplayName = "BattleMode")
};
