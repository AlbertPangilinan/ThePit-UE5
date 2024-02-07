// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"

// HUD Components
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Weapon
#include "Items/Weapons/Weapon.h"

// Interfaces
#include "Interfaces/InteractInterface.h"


void UPlayerOverlay::SetCrosshairSpread(float SpreadMultiplier, AWeapon* CurrentWeapon)
{
	if (CurrentWeapon == nullptr)
	{

	}
	else
	{
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
}

void UPlayerOverlay::SetCurrentAmmoCount(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon) CurrentAmmoCount->SetText(FText::FromString(FString::Printf(TEXT("%d | %d"), CurrentWeapon->GetCurrentAmmoCount(), CurrentWeapon->GetReserveAmmoCount())));
	else CurrentAmmoCount->SetText(FText());
}

void UPlayerOverlay::SetWeaponName(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon) WeaponName->SetText(FText::FromString(CurrentWeapon->GetWeaponName()));
	else WeaponName->SetText(FText());

}

void UPlayerOverlay::SetFireMode(AWeapon* CurrentWeapon)
{
	if (CurrentWeapon == nullptr)
	{
		FireMode->SetText(FText());
	}
	else
	{
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
}

void UPlayerOverlay::SetInteractAction(IInteractInterface* InteractableActor)
{
	InteractAction->SetText(FText::FromString(FString::Printf(TEXT("[F] %s"), *InteractableActor->GetInteractText())));
}

void UPlayerOverlay::ClearInteractAction()
{
	InteractAction->SetText(FText());
}
