// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerAnimInstance.h"

// Player Character
#include "Characters/PlayerCharacter.h"

// Weapon
#include "Items/Weapons/Weapon.h"

// Movement
#include "GameFramework/CharacterMovementComponent.h"


void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PlayerCharacter = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (PlayerCharacter)
	{
		PlayerCharacterMovement = PlayerCharacter->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (PlayerCharacterMovement)
	{
		PlayerMovementVector = PlayerCharacter->GetMovementVector();
		PlayerAimZ = PlayerCharacter->GetAimZ();
		GroundSpeed = PlayerCharacter->GetGroundSpeed();
		IsFalling = PlayerCharacterMovement->IsFalling();
		PlayerStance = PlayerCharacter->GetPlayerStance();
		AWeapon* ActiveWeapon = PlayerCharacter->GetActiveWeapon();
		if (ActiveWeapon) ActiveWeaponCurrentAmmoCount = ActiveWeapon->GetCurrentAmmoCount();
	}
}
