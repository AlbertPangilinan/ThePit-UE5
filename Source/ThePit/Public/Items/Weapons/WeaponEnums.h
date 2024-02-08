#pragma once

UENUM(BlueprintType)
enum class EWeaponClass : uint8
{
	EWC_Rifle UMETA(DisplayName = "Rifle"),
	EWC_Pistol UMETA(DisplayName = "Pistol"),
	EWC_Melee UMETA(DisplayName = "Melee"),
};

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	EWFM_SemiAuto UMETA(DisplayName = "Semi-Automatic"),
	EWFM_FullAuto UMETA(DisplayName = "Fully Automatic"),
};