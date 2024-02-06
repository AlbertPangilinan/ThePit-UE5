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
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::Fire()
{
	AActor* WeaponOwner = GetOwner();
	if (APlayerCharacter* PlayerCharacter = CastChecked<APlayerCharacter>(WeaponOwner))
	{
		PlayFiringFX();

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(GetOwner());

		FHitResult LineOfSightResult = PlayerCharacter->LineOfSightLineTrace(ObjectTypes, ActorsToIgnore);

		if (LineOfSightResult.IsValidBlockingHit())
		{
			FHitResult HitscanResult = HitscanLineTrace(PlayerCharacter, ObjectTypes, ActorsToIgnore, LineOfSightResult);

			if (HitscanResult.IsValidBlockingHit()) KnockOverTarget(HitscanResult);
		}		
	}
}

void AWeapon::Reload()
{
	int32 AmmoToReload = MagazineSize - CurrentAmmoCount;
	if (ReserveAmmoCount < AmmoToReload) AmmoToReload = ReserveAmmoCount;
	CurrentAmmoCount += AmmoToReload;
	ReserveAmmoCount -= AmmoToReload;
}

void AWeapon::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Interacted with: %s"), *GetWeaponName());
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

FHitResult AWeapon::HitscanLineTrace(APlayerCharacter* PlayerCharacter, TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TArray<AActor*> ActorsToIgnore, FHitResult LineOfSightResult)
{
	FHitResult HitscanResult;

	FVector HitscanStart = HitscanOrigin->GetComponentLocation();
	FVector HitscanEnd = CalculateTrajectory(PlayerCharacter, LineOfSightResult.ImpactPoint - HitscanStart);

	UKismetSystemLibrary::LineTraceSingleForObjects(this, HitscanStart, HitscanEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::ForOneFrame, HitscanResult, true, FColor::Red);

	return HitscanResult;
}

float AWeapon::CalculateAxisDeviation(APlayerCharacter* PlayerCharacter)
{
	return FMath::RandRange(-Spread, Spread) * PlayerCharacter->GetSpreadMultiplier();
}

FVector AWeapon::CalculateTrajectory(APlayerCharacter* PlayerCharacter, FVector HitscanPath)
{
	float SpreadX = CalculateAxisDeviation(PlayerCharacter);
	float SpreadY = CalculateAxisDeviation(PlayerCharacter);
	float SpreadZ = CalculateAxisDeviation(PlayerCharacter);

	return FRotator(SpreadX, SpreadY, SpreadZ).RotateVector(HitscanPath) * 5000.f;
}

void AWeapon::KnockOverTarget(FHitResult HitscanResult)
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
}

void AWeapon::PlayFiringFX()
{
	if (FiringSound) UGameplayStatics::PlaySoundAtLocation(this, FiringSound, GetActorLocation());
	if (MuzzleFlashEffect) UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, MuzzleFlashEffect, MuzzleFlashOrigin->GetComponentLocation(), MuzzleFlashOrigin->GetComponentRotation());
	CurrentAmmoCount -= 1;
}
