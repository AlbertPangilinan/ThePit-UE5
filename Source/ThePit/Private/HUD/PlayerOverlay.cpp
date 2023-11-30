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
