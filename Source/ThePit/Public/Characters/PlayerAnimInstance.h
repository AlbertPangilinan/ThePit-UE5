// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	UCharacterMovementComponent* PlayerCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	FVector2D PlayerMovementVector;

	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	bool IsFalling;

};
