// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"

// HUD Components
#include "Components/TextBlock.h"

// Weapon
#include "Items/Weapons/Weapon.h"

void UPlayerOverlay::SetAmmoCount(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon) AmmoCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentWeapon->GetAmmoCount(), CurrentWeapon->GetMagazineSize())));
}

void UPlayerOverlay::SetWeaponName(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon) WeaponName->SetText(FText::FromString(CurrentWeapon->GetWeaponName()));
}

void UPlayerOverlay::SetFireMode(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon == nullptr) return;

	EWeaponFireMode CurrentFireMode = CurrentWeapon->GetFireMode();

	switch (CurrentFireMode)
	{
	case EWeaponFireMode::EWFM_FullAuto:
		FireMode->SetText(FText::FromString(FString::Printf(TEXT("Full Auto"))));
		break;
	case EWeaponFireMode::EWFM_SemiAuto:
		FireMode->SetText(FText::FromString(FString::Printf(TEXT("Semi Auto"))));
		break;
	default:
		break;
	}

}
