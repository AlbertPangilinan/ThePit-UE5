// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/EnvActor.h"

// Sets default values
AEnvActor::AEnvActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ActorMesh"));
	ActorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ActorMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetRootComponent(ActorMesh);
}

// Called when the game starts or when spawned
void AEnvActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnvActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

