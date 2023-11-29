// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


class USphereComponent;


UCLASS()
class THEPIT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem(); // Sets default values for this actor's properties
	virtual void Tick(float DeltaTime) override; // Called every frame


protected:
	virtual void BeginPlay() override; // Called when the game starts or when spawned

	// Variables
	// Components
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;


private:


};
