// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

// Components
#include "Components/SphereComponent.h"

// Player Character
#include "Characters/PlayerCharacter.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Mesh Setup
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetRootComponent(ItemMesh);

	// Interact Radius Setup
	InteractRadius = CreateDefaultSubobject<USphereComponent>(TEXT("InteractRadius"));
	InteractRadius->SetSphereRadius(64.f);
	InteractRadius->SetupAttachment(GetRootComponent());

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
