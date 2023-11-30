// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

// HUD Components
class UTextBlock;


/**
 * 
 */
UCLASS()
class THEPIT_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Variables
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AmmoCount;


private:
	// Functions
	void SetAmmoCount(int32 CurrentAmmoCount);
};
