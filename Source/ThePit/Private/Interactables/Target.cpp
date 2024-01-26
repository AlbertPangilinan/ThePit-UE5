// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Target.h"

// Kismet
#include "Kismet/KismetMathLibrary.h"

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
	SetRootComponent(TargetMesh);
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const int32 NumTargets = MovementTargets.Num();
	if (!IsHit && MovementTarget && NumTargets > 0)
	{
		if (GetActorLocation() == MovementTarget->GetActorLocation()) SelectNewMovementTarget();
		MoveToTarget(DeltaTime);
	}
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();

}

void ATarget::SelectNewMovementTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Select New"));
	TArray<AActor*> ValidTargets;
	for (AActor* Target : MovementTargets)
	{
		if (Target != MovementTarget) ValidTargets.AddUnique(Target);
	}

	const int32 NumValidTargets = ValidTargets.Num();
	if (NumValidTargets > 0)
	{
		const int32 NewTarget = FMath::FRandRange(0, NumValidTargets - 1);
		MovementTarget = ValidTargets[NewTarget];
	}
}

void ATarget::MoveToTarget(float DeltaTime)
{
	SetActorLocation(UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), MovementTarget->GetActorLocation(), DeltaTime, 100.f));
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
