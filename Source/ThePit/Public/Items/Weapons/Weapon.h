// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

// Combat
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
	void PlayFiringSound();

	// Variables
	// Weapon Properties
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitscanOrigin;


	// Combat
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float RateOfFire = 857.f; // RPM

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* FiringSound;


public:
	FORCEINLINE float GetRateOfFireSeconds() { return 60.f / RateOfFire; }

};
