// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"

// Enums
#include "WeaponEnums.h"

#include "Weapon.generated.h"

// Combat
class UNiagaraSystem;
class USoundBase;

class APlayerCharacter;


UCLASS()
class THEPIT_API AWeapon : public AItem
{
	GENERATED_BODY()
	
public:
	AWeapon();

	// Functions
	// Combat
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void Fire();
	void Reload();


protected:


private:
	// Functions
	// Combat
	FHitResult HitscanLineTrace(APlayerCharacter* PlayerCharacter, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TArray<AActor*> ActorsToIgnore, FHitResult LineOfSightResult);
	float CalculateAxisDeviation(APlayerCharacter* PlayerCharacter);
	FVector CalculateTrajectory(APlayerCharacter* PlayerCharacter, FVector HitscanPath);
	void KnockOverTarget(FHitResult HitscanResult);
	void PlayFiringFX();


	// Variables
	// Weapon Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitscanOrigin;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* MuzzleFlashOrigin;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* LeftHandPlacement;


	// Weapon Properties
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FString WeaponName = "WeaponName";
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float RateOfFire = 600.f; // RPM

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	EWeaponFireMode FireMode = EWeaponFireMode::EWFM_FullAuto;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Spread = 2.5f; // Deviation from crosshair centre in degrees

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 CurrentAmmoCount = 30;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 ReserveAmmoCount = 60;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 MagazineSize = 30;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* FiringSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UNiagaraSystem* MuzzleFlashEffect;


public:
	FORCEINLINE FString GetWeaponName() const { return WeaponName;  }
	FORCEINLINE float GetRateOfFireSeconds() { return 60.f / RateOfFire; }
	FORCEINLINE EWeaponFireMode GetFireMode() const { return FireMode; }
	FORCEINLINE void SetFireMode(EWeaponFireMode NewFireMode) { FireMode = NewFireMode; }
	FORCEINLINE float GetSpread() const { return Spread; }
	FORCEINLINE int32 GetCurrentAmmoCount() const { return CurrentAmmoCount; }
	FORCEINLINE int32 GetReserveAmmoCount() const { return ReserveAmmoCount; }
	FORCEINLINE int32 GetMagazineSize() const { return MagazineSize; }
};
