#pragma once

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	EPS_Standing UMETA(DisplayName = "Standing"),
	EPS_Crouching UMETA(DisplayName = "Crouching"),
	EPS_Prone UMETA(DisplayName = "Prone"),
};

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPC_Hipfire UMETA(DisplayName = "Hipfire"),
	EPC_ADS UMETA(DisplayName = "ADS"),
};
