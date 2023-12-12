// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Target.h"

// Niagara
#include "NiagaraFunctionLibrary.h"


// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TargetMesh"));
	TargetMesh->bUseDefaultCollision = true;
	//TargetMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//TargetMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetRootComponent(TargetMesh);
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

}

void ATarget::SpawnBulletImpactSystem(FVector ImpactPoint)
{
	if (BulletImpactEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, BulletImpactEffect, ImpactPoint, GetActorRotation());
}

void ATarget::KnockOver()
{
	if (!IsHit)
	{
		SetActorRotation(GetActorRotation() + FRotator(0.f, 0.f, 90.f));
		IsHit = true;
	}
}
