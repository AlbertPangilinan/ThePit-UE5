// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoBox.generated.h"


class USphereComponent;


UCLASS()
class THEPIT_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	AAmmoBox(); // Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame


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
	// Functions
	// Interact
	void RefillAmmo();

};
