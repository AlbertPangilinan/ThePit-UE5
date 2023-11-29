// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */
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
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HitscanOrigin;

};
