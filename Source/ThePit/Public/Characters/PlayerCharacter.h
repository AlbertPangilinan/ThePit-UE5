// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Enhanced Input
#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"


// Camera
class UCameraComponent;
class USpringArmComponent;

// Enhanced Input
class UInputMappingContext;
class UInputAction;

// Combat
class AWeapon;


UCLASS()
class THEPIT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter(); // Sets default values for this character's properties

	virtual void Tick(float DeltaTime) override; // Called every frame
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // Called to bind functionality to input

	// Combat
	void EquipWeapon();

protected:
	// Functions
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	// Enhanced Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Attack();
	void StartAttackTimer();
	void ClearAttackTimer();

	// Variables
	// Enhanced Input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;


	// Character State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D MovementVector;


	// Combat
	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* PrimaryWeapon;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* SecondaryWeapon;

	// TEMP
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> PrimaryWeaponClass;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> SecondaryWeaponClass;
	// TEMP


private:
	// Functions
	void UpdateWeaponHUD();


	// Variables
	// Camera
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;


	// Combat
	FTimerHandle AttackTimer;

public:
	FORCEINLINE UCameraComponent* GetCamera() const { return ViewCamera; }

};
