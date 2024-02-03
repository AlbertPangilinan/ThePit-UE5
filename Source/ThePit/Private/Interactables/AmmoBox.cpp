// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/AmmoBox.h"

// Components
#include "Components/SphereComponent.h"

// Player Character
#include "Characters/PlayerCharacter.h"

// Combat
#include "Items/Weapons/Weapon.h"


// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	BoxMesh->bUseDefaultCollision = true;
	SetRootComponent(BoxMesh);

	// Interact Radius Setup
	InteractRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRadius"));
	InteractRadius->SetupAttachment(GetRootComponent());

}

void AAmmoBox::Interact()
{
	if (ActiveWeapon && ActiveWeapon->GetReserveAmmoCount() < ActiveWeapon->GetMaxReserveAmmo()) ActiveWeapon->SetReserveAmmoCount(ActiveWeapon->GetMaxReserveAmmo());
}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
	
	InteractRadius->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnSphereBeginOverlap);
	InteractRadius->OnComponentEndOverlap.AddDynamic(this, &AAmmoBox::OnSphereEndOverlap);
}

void AAmmoBox::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->SetOverlappingActor(this);
		ActiveWeapon = PlayerCharacter->GetActiveWeapon();
	}
}

void AAmmoBox::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->ClearOverlappingActor();
		ActiveWeapon = nullptr;
	}
}
