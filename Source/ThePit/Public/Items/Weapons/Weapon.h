// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"

// Interfaces
#include "Interfaces/InteractInterface.h"

// Enums
#include "WeaponEnums.h"

#include "Weapon.generated.h"

// Combat
class UNiagaraSystem;
class USoundBase;

class APlayerCharacter;


UCLASS()
class THEPIT_API AWeapon : public AItem, public IInteractInterface
{
	GENERATED_BODY()
	
public:
	AWeapon(); // Sets default values for this actor's properties

	// Functions
	// Interact
	virtual void Interact() override;

	// Combat
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);
	void Fire();
	void Reload();


protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	// Functions
	// Interact
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


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
	int32 MaxReserveAmmo = 60;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* FiringSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	UNiagaraSystem* MuzzleFlashEffect;

	// Overlapping Player
	APlayerCharacter* OverlappingPlayerCharacter;


public:
	FORCEINLINE FString GetWeaponName() const { return WeaponName;  }
	FORCEINLINE float GetRateOfFireSeconds() { return 60.f / RateOfFire; }
	FORCEINLINE EWeaponFireMode GetFireMode() const { return FireMode; }
	FORCEINLINE void SetFireMode(EWeaponFireMode NewFireMode) { FireMode = NewFireMode; }
	FORCEINLINE float GetSpread() const { return Spread; }
	FORCEINLINE int32 GetCurrentAmmoCount() const { return CurrentAmmoCount; }
	FORCEINLINE int32 GetReserveAmmoCount() const { return ReserveAmmoCount; }
	FORCEINLINE void SetReserveAmmoCount(int32 NewAmmoCount) { ReserveAmmoCount = NewAmmoCount; }
	FORCEINLINE int32 GetMagazineSize() const { return MagazineSize; }
	FORCEINLINE int32 GetMaxReserveAmmo() const { return MaxReserveAmmo; }
	FORCEINLINE virtual FString GetInteractText() override { return WeaponName; }
};
