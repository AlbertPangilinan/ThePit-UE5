// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Interfaces
#include "Interfaces/InteractInterface.h"

#include "AmmoBox.generated.h"


// Components
class USphereComponent;

// Combat
class AWeapon;


UCLASS()
class THEPIT_API AAmmoBox : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AAmmoBox(); // Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame
	virtual void Interact() override;


protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	// Functions
	// Interact
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Variables
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BoxMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* InteractRadius;


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
