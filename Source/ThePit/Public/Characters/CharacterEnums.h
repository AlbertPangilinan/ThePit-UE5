#pragma once

UENUM(BlueprintType)
enum class EPlayerStance : uint8
{
	EPS_Standing UMETA(DisplayName = "Standing"),
	EPS_Crouching UMETA(DisplayName = "Crouching"),
	EPS_Prone UMETA(DisplayName = "Prone"),
};