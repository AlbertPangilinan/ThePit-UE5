// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"

// HUD
#include "HUD/PlayerOverlay.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && PlayerOverlayClass)
		{
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, PlayerOverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}
