#pragma once

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "Unequipped"),
	ECS_Equipped UMETA(DisplayName = "Equipped")
};