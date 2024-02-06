// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

// Components
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

// Player Character
#include "Characters/PlayerCharacter.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(ItemMesh);

	// Interact Radius Setup
	InteractRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRadius"));
	InteractRadius->SetSphereRadius(128.f);
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

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->AddOverlappingActor(this);
		OverlappingPlayerCharacter = PlayerCharacter;
	}
}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(OtherActor))
	{
		PlayerCharacter->RemoveOverlappingActor(this);
		OverlappingPlayerCharacter = nullptr;
	}
}

void AItem::Interact()
{
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	InteractRadius->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
	InteractRadius->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);

}
