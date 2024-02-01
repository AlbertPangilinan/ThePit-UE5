// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/AmmoBox.h"

// Components
#include "Components/SphereComponent.h"


// Sets default values
AAmmoBox::AAmmoBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	BoxMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(BoxMesh);

	// Interact Radius Setup
	InteractRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRadius"));
	InteractRadius->SetupAttachment(GetRootComponent());

}

// Called every frame
void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called when the game starts or when spawned
void AAmmoBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAmmoBox::RefillAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("Ammo Refilled"));
}
