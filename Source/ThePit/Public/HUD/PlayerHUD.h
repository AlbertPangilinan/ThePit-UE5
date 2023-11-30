// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UPlayerOverlay;


/**
 * 
 */
UCLASS()
class THEPIT_API APlayerHUD : public AHUD
{
	GENERATED_BODY()


protected:
	// Functions
	virtual void BeginPlay() override;


private:
	// Variables
	// HUD Components
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UPlayerOverlay> PlayerOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
