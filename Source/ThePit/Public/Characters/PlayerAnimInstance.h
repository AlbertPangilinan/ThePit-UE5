// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

// Enums
#include "Characters/CharacterEnums.h"

#include "PlayerAnimInstance.generated.h"

class APlayerCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class THEPIT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	// Functions
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;


	// Variables
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	FVector2D PlayerMovementVector;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = State)
	EPlayerStance PlayerStance;

	UPROPERTY(BlueprintReadOnly, Category = State)
	int32 ActiveWeaponAmmoCount;

};
