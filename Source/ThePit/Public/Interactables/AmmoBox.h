// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Interfaces
#include "Interfaces/InteractInterface.h"

#include "AmmoBox.generated.h"


// Components
class USphereComponent;
class UBoxComponent;

// Combat
class AWeapon;


UCLASS()
class THEPIT_API AAmmoBox : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AAmmoBox(); // Sets default values for this actor's properties

	// Functions
	// Interact
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	virtual void Interact() override;


protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned


	// Variables
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* InteractRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* LineOfSightBounds;


private:	
	// Variables
	// Weapon
	AWeapon* ActiveWeapon;

	// Interact
	UPROPERTY(EditAnywhere, Category = "Interact Properties")
	FString InteractText = "";


public:
	FORCEINLINE virtual FString GetInteractText() override { return InteractText; }


};
