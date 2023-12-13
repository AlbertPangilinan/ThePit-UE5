#pragma once

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	EWFM_SemiAuto UMETA(DisplayName = "Semi-Automatic"),
	EWFM_FullAuto UMETA(DisplayName = "Fully Automatic"),
};