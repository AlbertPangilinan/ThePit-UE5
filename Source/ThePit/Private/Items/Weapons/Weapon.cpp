// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"

// Player Character
#include "Characters/PlayerCharacter.h"

// Camera
#include "Camera/CameraComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"
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
		UCameraComponent* ViewCamera = PlayerCharacter->GetCamera();
		
		const FVector CameraLocation = ViewCamera->GetComponentLocation();
		const FVector CameraRotation = ViewCamera->GetComponentRotation().GetNormalized().Vector();
		const FVector Start = CameraLocation + CameraRotation;
		const FVector End = Start + CameraRotation * 5000.f;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		FHitResult LineOfSightResult;
		FHitResult HitscanResult;

		UKismetSystemLibrary::LineTraceSingleForObjects(this, Start, End, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, LineOfSightResult, true, FColor::Blue);

		if (LineOfSightResult.IsValidBlockingHit())
		{
			UKismetSystemLibrary::LineTraceSingleForObjects(this, HitscanOrigin->GetComponentLocation(), LineOfSightResult.ImpactPoint, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitscanResult, true, FColor::Red);
			PlayFiringSound();

			if (HitscanResult.IsValidBlockingHit())
			{
				DrawDebugSphere(GetWorld(), HitscanResult.ImpactPoint, 5.f, 12, FColor::Green, false, 1.f);
			}
		}
		
	}
}

void AWeapon::PlayFiringSound()
{
	if (FiringSound) UGameplayStatics::PlaySoundAtLocation(this, FiringSound, GetActorLocation());
}
