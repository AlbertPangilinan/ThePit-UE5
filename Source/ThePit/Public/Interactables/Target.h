// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"


// Combat
class UNiagaraSystem;


UCLASS()
class THEPIT_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ATarget(); // Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame


	// Functions
	// Interact
	void SpawnBulletImpactSystem(FVector ImpactPoint);
	void KnockOver();


protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned


	// Variables
	// Components
	UPROPERTY(EditAnywhere, Category = "Target Properties")
	UStaticMeshComponent* TargetMesh;


	UPROPERTY(EditAnywhere, Category = "Target Properties")
	UNiagaraSystem* BulletImpactEffect;


private:
	// Functions
	// Navigation
	void SelectNewMovementTarget();
	void MoveToTarget(float DeltaTime);


	// Variables
	// Hit Detection
	bool IsHit = false;

	// Movement
	UPROPERTY(EditInstanceOnly, Category = "Target Movement")
	AActor* MovementTarget;

	UPROPERTY(EditInstanceOnly, Category = "Target Movement")
	TArray<AActor*> MovementTargets;
};
