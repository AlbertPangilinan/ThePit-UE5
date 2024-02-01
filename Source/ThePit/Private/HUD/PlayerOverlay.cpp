// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"

// HUD Components
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Weapon
#include "Items/Weapons/Weapon.h"

void UPlayerOverlay::SetCrosshairSpread(float SpreadMultiplier, AWeapon* CurrentWeapon)
{
	if (CurrentWeapon == nullptr) return;

	float CrosshairSpread = CurrentWeapon->GetSpread() * SpreadMultiplier * CrosshairOffset;

	UCanvasPanelSlot* CrosshairTopSlot = CastChecked<UCanvasPanelSlot>(CrosshairTop->Slot);
	UCanvasPanelSlot* CrosshairBotSlot = CastChecked<UCanvasPanelSlot>(CrosshairBot->Slot);
	UCanvasPanelSlot* CrosshairLeftSlot = CastChecked<UCanvasPanelSlot>(CrosshairLeft->Slot);
	UCanvasPanelSlot* CrosshairRightSlot = CastChecked<UCanvasPanelSlot>(CrosshairRight->Slot);

	CrosshairTopSlot->SetPosition(FVector2D(0.f, -CrosshairSpread));
	CrosshairBotSlot->SetPosition(FVector2D(0.f, CrosshairSpread));
	CrosshairLeftSlot->SetPosition(FVector2D(-CrosshairSpread, 0.f));
	CrosshairRightSlot->SetPosition(FVector2D(CrosshairSpread, 0.f));
}

void UPlayerOverlay::SetCurrentAmmoCount(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon) CurrentAmmoCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), CurrentWeapon->GetCurrentAmmoCount(), CurrentWeapon->GetMagazineSize())));
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
