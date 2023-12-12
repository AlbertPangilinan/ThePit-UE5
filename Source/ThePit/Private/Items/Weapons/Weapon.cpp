// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"

// Player Character
#include "Characters/PlayerCharacter.h"

// Movement
#include "GameFramework/CharacterMovementComponent.h"

// Target
#include "Interactables/Target.h"

// Camera
#include "Camera/CameraComponent.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// Niagara
#include "NiagaraFunctionLibrary.h"


// Sets default values
AWeapon::AWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HitscanOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("HitscanOrigin"));
	HitscanOrigin->SetupAttachment(GetRootComponent());

	MuzzleFlashOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleFlashOrigin"));
	MuzzleFlashOrigin->SetupAttachment(GetRootComponent());

	LeftHandPlacement = CreateDefaultSubobject<USceneComponent>(TEXT("LeftHandPlacement"));
	LeftHandPlacement->SetupAttachment(GetRootComponent());
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
		const FVector LineOfSightStart = CameraLocation + CameraRotation;
		const FVector LineOfSightEnd = LineOfSightStart + CameraRotation * 5000.f;

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		FHitResult LineOfSightResult;
		FHitResult HitscanResult;

		UKismetSystemLibrary::LineTraceSingleForObjects(this, LineOfSightStart, LineOfSightEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, LineOfSightResult, true, FColor::Blue);

		PlayFiringSound();
		SpawnMuzzleFlashSystem();
		AmmoCount -= 1;

		if (LineOfSightResult.IsValidBlockingHit())
		{
			FVector HitscanStart = HitscanOrigin->GetComponentLocation();

			double SpreadMultiplier = FMath::Max(1.0, (LineOfSightResult.ImpactPoint - HitscanStart).Length() * 0.002);

			if (UKismetMathLibrary::VSizeXY(PlayerCharacter->GetCharacterMovement()->Velocity) > 0.f) SpreadMultiplier *= 1.25;
			if (PlayerCharacter->GetPlayerStance() == EPlayerStance::EPS_Crouching) SpreadMultiplier *= 0.7;
			if (PlayerCharacter->GetPlayerAimState() == EPlayerAimState::EPAS_ADS) SpreadMultiplier *= 0.5;

			float SpreadX = FMath::RandRange(-Spread, Spread) * SpreadMultiplier;
			float SpreadY = FMath::RandRange(-Spread, Spread) * SpreadMultiplier;
			float SpreadZ = FMath::RandRange(-Spread, Spread) * SpreadMultiplier;

			FVector LineOfSightWithSpread = LineOfSightResult.ImpactPoint + FVector(SpreadX, SpreadY, SpreadZ);
			FVector HitscanEnd = (LineOfSightWithSpread - HitscanStart) * 5000.f;

			UKismetSystemLibrary::LineTraceSingleForObjects(this, HitscanStart, HitscanEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitscanResult, true, FColor::Red);

			if (HitscanResult.IsValidBlockingHit())
			{
				if (ATarget* Target = Cast<ATarget>(HitscanResult.GetActor()))
				{
					Target->SpawnBulletImpactSystem(HitscanResult.ImpactPoint);
					Target->KnockOver();
				}
				else
				{
					DrawDebugSphere(GetWorld(), HitscanResult.ImpactPoint, 3.f, 12, FColor::Green, false, 1000.f);
				}
				UE_LOG(LogTemp, Warning, TEXT("Fire"));
			}
		}
		
	}
}

void AWeapon::Reload()
{
	AmmoCount = MagazineSize;
}

void AWeapon::PlayFiringSound()
{
	if (FiringSound) UGameplayStatics::PlaySoundAtLocation(this, FiringSound, GetActorLocation());
}

void AWeapon::SpawnMuzzleFlashSystem()
{
	if (MuzzleFlashEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleFlashEffect, MuzzleFlashOrigin->GetComponentLocation(), MuzzleFlashOrigin->GetComponentRotation());
}
