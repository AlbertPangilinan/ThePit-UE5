// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

// Widget
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"

#include "PlayerOverlay.generated.h"


// HUD Components
class UImage;
class UTextBlock;

// Weapon
class AWeapon;

// Interact
class IInteractInterface;

/**
 * 
 */
UCLASS()
class THEPIT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Functions
	// Crosshair
	void SetCrosshairSpread(float SpreadMultiplier, AWeapon* CurrentWeapon);
	void ClearCrosshairSpread();

	// Weapon
	void SetCurrentAmmoCount(AWeapon* CurrentWeapon);
	void ClearCurrentAmmoCount();
	void SetWeaponName(AWeapon* CurrentWeapon);
	void ClearWeaponName();
	void SetFireMode(AWeapon* CurrentWeapon);
	void ClearFireMode();

	// Interact
	void SetInteractAction(IInteractInterface* InteractableActor);
	void ClearInteractAction();

	// Variables
	// Crosshair
	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairTop;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairBot;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairLeft;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairRight;

	// Weapon
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FireMode;

	// Interact
	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractAction;


private:
	// Variables
	const float CrosshairOffset = 30.f;


};
