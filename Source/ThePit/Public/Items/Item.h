// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Interfaces
#include "Interfaces/InteractInterface.h"

#include "Item.generated.h"


// Components
class USphereComponent;
class UBoxComponent;

// Player Character
class APlayerCharacter;


UCLASS()
class THEPIT_API AItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AItem(); // Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame

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
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* InteractRadius;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* LineOfSightBounds;

	// Item Properties
	UPROPERTY(EditAnywhere, Category = "Item Properties")
	FString ItemName = "ItemName";

	// Overlapping Player
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APlayerCharacter* OverlappingPlayerCharacter;


private:


public:
	FORCEINLINE virtual FString GetInteractText() override { return ItemName; }

};
