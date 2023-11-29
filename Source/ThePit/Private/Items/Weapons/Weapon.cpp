// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"

// Player Character
#include "Characters/PlayerCharacter.h"

// Camera
#include "Camera/CameraComponent.h"

// Kismet System Library
#include "Kismet/KismetSystemLibrary.h"

AWeapon::AWeapon()
{
	HitscanOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	HitscanOrigin->SetupAttachment(GetRootComponent());
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (APlayerCharacter* PlayerCharacter = CastChecked<APlayerCharacter>(WeaponOwner))
	{
		if (UWorld* World = GetWorld())
		{

			UCameraComponent* ViewCamera = PlayerCharacter->GetCamera();
		
			const FVector CameraLocation = ViewCamera->GetComponentLocation();
			const FVector CameraRotation = ViewCamera->GetComponentRotation().GetNormalized().Vector();
			const FVector Start = CameraLocation + CameraRotation;
			const FVector End = Start + CameraRotation * 5000.f;

			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(GetOwner());

			FHitResult HitResult;

			UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::Persistent, HitResult, true, FColor::Blue);

			if (HitResult.IsValidBlockingHit())
			{
				DrawDebugLine(World, HitscanOrigin->GetComponentLocation(), HitResult.ImpactPoint, FColor::Red, true);
				DrawDebugSphere(World, HitResult.ImpactPoint, 25.f, 24, FColor::Green, true);
			}
		}
	}
}
