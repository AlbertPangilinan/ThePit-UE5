// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/AmmoBox.h"

// Components
#include "Components/SphereComponent.h"

// Player Character
#include "Characters/PlayerCharacter.h"


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

// Called every frame
void AAmmoBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoBox::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("Ammo Refilled"));
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
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor)) PlayerCharacter->SetOverlappingActor(this);
}

void AAmmoBox::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor)) PlayerCharacter->ClearOverlappingActor();
}
