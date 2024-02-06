// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Enums
#include "CharacterEnums.h"

// Enhanced Input
#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"


// Camera
class UCameraComponent;
class USpringArmComponent;

// Enhanced Input
class UInputMappingContext;
class UInputAction;

// Animation
class UAnimMontage;

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

	// Functions
	// Camera
	FVector GetCameraLocation();
	FVector GetCameraRotation();

	// Utility
	FHitResult LineOfSightLineTrace(TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TArray<AActor*> ActorsToIgnore);
	double GetGroundSpeed();

protected:
	// Functions
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	// Enhanced Input
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	virtual void Jump() override;
	void ChangeStance();
	void Attack();
	void AttackFullAuto();
	void AttackSemiAuto();
	void ToggleADS();
	void CycleFireMode();
	void SwitchWeapon();
	void StartAttackTimer();
	void ClearAttackTimer();
	void Interact();

	// Animation
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	// Combat
	void CalculateSpreadMultiplier();
	void EquipWeapon();
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void StartWeaponSwitch();

	UFUNCTION(BlueprintCallable)
	void EndWeaponSwitch();

	UFUNCTION(BlueprintCallable)
	void StartReloadActiveWeapon();

	UFUNCTION(BlueprintCallable)
	void EndReloadActiveWeapon();


	// Variables
	// Enhanced Input
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* PlayerContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ChangeStanceAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ADSAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CycleFireModeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SwitchWeaponAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;


	// Player State
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerStance PlayerStance = EPlayerStance::EPS_Standing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerAimState PlayerAimState = EPlayerAimState::EPAS_Hipfire;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EPlayerCombatState PlayerCombatState = EPlayerCombatState::EPCS_Aiming;


	// Movement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D MovementVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AimZ;


	// Combat
	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* ActiveWeapon;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* EquippedWeapon1;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	AWeapon* EquippedWeapon2;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	float SpreadMultiplier = 1.f;

	UPROPERTY(VisibleAnywhere, Category = Combat)
	float MaxRange = 10000.f;

	// TEMP
	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> Weapon1Class;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AWeapon> Weapon2Class;
	// TEMP


private:
	// Functions
	// Combat
	void UpdateWeaponHUD();
	void SwitchWeaponSockets();

	// Interact
	void UpdateInteractHUD();
	void GetLineOfSightActor();
	bool CanInteract();
	

	// Variables
	// Camera
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	float TargetCameraPosition;
	float TargetCameraZoom;


	// Animation
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* CombatMontage;


	// Combat
	FTimerHandle AttackTimer;


	// Interact
	UPROPERTY(VisibleInstanceOnly, Category = Interact)
	TArray<AActor*> OverlappingActors;
	
	UPROPERTY(VisibleInstanceOnly, Category = Interact)
	AActor* LineOfSightActor;


public:
	FORCEINLINE EPlayerStance GetPlayerStance() const { return PlayerStance; }
	FORCEINLINE EPlayerAimState GetPlayerAimState() const { return PlayerAimState; }
	FORCEINLINE EPlayerCombatState GetPlayerCombatState() const { return PlayerCombatState; }
	FORCEINLINE float GetSpreadMultiplier() const { return SpreadMultiplier; }
	FORCEINLINE FVector2D GetMovementVector() const { return MovementVector; }
	FORCEINLINE float GetAimZ() const { return AimZ; }
	FORCEINLINE void AddOverlappingActor(AActor* Actor) { OverlappingActors.AddUnique(Actor); }
	FORCEINLINE void RemoveOverlappingActor(AActor* Actor) { OverlappingActors.RemoveSingle(Actor); }
	FORCEINLINE AWeapon* GetActiveWeapon() { return ActiveWeapon; }
	FORCEINLINE void SetActiveWeapon(AWeapon* Weapon) { ActiveWeapon = Weapon; }
};
