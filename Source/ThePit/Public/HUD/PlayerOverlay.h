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


/**
 * 
 */
UCLASS()
class THEPIT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Functions
	void SetCrosshairSpread(float SpreadMultiplier, AWeapon* CurrentWeapon);
	void SetCurrentAmmoCount(AWeapon* CurrentWeapon);
	void SetWeaponName(AWeapon* CurrentWeapon);
	void SetFireMode(AWeapon* CurrentWeapon);


	// Variables
	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairTop;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairBot;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairLeft;

	UPROPERTY(meta = (BindWidget))
	UImage* CrosshairRight;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurrentAmmoCount;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WeaponName;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* FireMode;

private:
	// Variables
	const float CrosshairOffset = 30.f;


};
