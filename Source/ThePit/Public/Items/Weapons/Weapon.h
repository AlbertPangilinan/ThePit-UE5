// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

// Combat
class UNiagaraSystem;
class USoundBase;


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

protected:



private:
	// Functions
	// Combat
	void PlayFiringSound();
	void SpawnMuzzleFlashSystem();

	// Variables
	// Weapon Components
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitscanOrigin;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* MuzzleFlashOrigin;


	// Weapon Properties
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 15.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float RateOfFire = 750.f; // RPM

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 MagazineSize = 30;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	int32 AmmoCount = 30;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* FiringSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UNiagaraSystem* MuzzleFlashEffect;


public:
	FORCEINLINE float GetRateOfFireSeconds() { return 60.f / RateOfFire; }
	FORCEINLINE int32 GetAmmoCount() const { return AmmoCount; }
	FORCEINLINE int32 GetMagazineSize() const { return MagazineSize; }
};
