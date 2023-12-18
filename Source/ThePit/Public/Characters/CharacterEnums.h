#pragma once

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	EPS_Standing UMETA(DisplayName = "Standing"),
	EPS_Crouching UMETA(DisplayName = "Crouching"),
	EPS_Prone UMETA(DisplayName = "Prone"),
};

UENUM(BlueprintType)
enum class EPlayerAimState : uint8
{
	EPAS_Hipfire UMETA(DisplayName = "Hipfire"),
	EPAS_ADS UMETA(DisplayName = "ADS"),
};

UENUM(BlueprintType)
enum class EPlayerCombatState : uint8
{
	EPCS_Aiming UMETA(DisplayName = "Aiming"),
	EPCS_Firing UMETA(DisplayName = "Firing"),
	EPCS_Reloading UMETA(DisplayName = "Reloading"),
	EPCS_SwitchingWeapons UMETA(DisplayName = "Switching Weapons"),
};
