// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"


// HUD Components
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
	void SetAmmoCount(AWeapon* CurrentWeapon);

	// Variables
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoCount;


};
