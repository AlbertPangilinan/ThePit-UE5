// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/AmmoBox.h"

// Components
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

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
	InteractRadius->SetSphereRadius(72.f);
	InteractRadius->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	InteractRadius->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractRadius->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	InteractRadius->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractRadius->SetupAttachment(GetRootComponent());

	// Line of Sight Bounds Setup
	LineOfSightBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("LineOfSightBounds"));
	LineOfSightBounds->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	LineOfSightBounds->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LineOfSightBounds->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	LineOfSightBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LineOfSightBounds->SetupAttachment(GetRootComponent());
}

void AAmmoBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->AddOverlappingActor(this);
		ActiveWeapon = PlayerCharacter->GetActiveWeapon();
	}
}

void AAmmoBox::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->RemoveOverlappingActor(this);
		ActiveWeapon = nullptr;
	}
}

void AAmmoBox::Interact()
{
	if (ActiveWeapon && ActiveWeapon->GetReserveAmmoCount() < ActiveWeapon->GetMaxReserveAmmo()) ActiveWeapon->SetReserveAmmoCount(ActiveWeapon->GetMaxReserveAmmo());
}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();

	InteractRadius->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnBeginOverlap);
	InteractRadius->OnComponentEndOverlap.AddDynamic(this, &AAmmoBox::OnEndOverlap);
}
