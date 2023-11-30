// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"

// HUD Components
#include "Components/TextBlock.h"

void UPlayerOverlay::SetAmmoCount(int32 CurrentAmmoCount)
{
	if (AmmoCount) AmmoCount->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentAmmoCount)));
}
