// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Characters/CharacterEnums.h"

// Enhanced Input
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Combat
#include "Items/Weapons/Weapon.h"

// HUD
#include "HUD/PlayerHUD.h"
#include "HUD/PlayerOverlay.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Camera Setup
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 250.f;
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 75.f);

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(ChangeStanceAction, ETriggerEvent::Started, this, &APlayerCharacter::ChangeStance);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAttackTimer);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::ClearAttackTimer);
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchWeapon);
	}
}

void APlayerCharacter::EquipWeapon()
{
	// TEMP: Equip Weapons
	if (UWorld* World = GetWorld())
	{
		if (Weapon1Class)
		{
			AWeapon* Weapon1 = World->SpawnActor<AWeapon>(Weapon1Class);
			Weapon1->Equip(GetMesh(), FName("RightHandSocket"), this, this);
			EquippedWeapon1 = Weapon1;
		}

		if (Weapon2Class)
		{
			AWeapon* Weapon2 = World->SpawnActor<AWeapon>(Weapon2Class);
			Weapon2->Equip(GetMesh(), FName("BackSocket"), this, this);
			EquippedWeapon2 = Weapon2;
		}
	}
	ActiveWeapon = EquippedWeapon1;
	UpdateWeaponHUD();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	EquipWeapon();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Setup Enhanced Input
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerContext, 0);
		}		
	}	
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, MovementVector.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APlayerCharacter::Jump()
{
	if (PlayerStance != EPlayerStance::EPS_Standing) return;
	Super::Jump();
}

void APlayerCharacter::ChangeStance()
{
	switch (PlayerStance)
	{
		case EPlayerStance::EPS_Crouching:
			PlayerStance = EPlayerStance::EPS_Standing;
			break;
		case EPlayerStance::EPS_Standing:
			PlayerStance = EPlayerStance::EPS_Crouching;
			break;
		default:
			break;
	}
}

void APlayerCharacter::Attack()
{
	if (ActiveWeapon == nullptr || ActiveWeapon->GetAmmoCount() <= 0) return;
	ActiveWeapon->Fire();
	UpdateWeaponHUD();
}

void APlayerCharacter::SwitchWeapon()
{
	if (ActiveWeapon == EquippedWeapon1)
	{
		ActiveWeapon = EquippedWeapon2;
		EquippedWeapon2->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon1->Equip(GetMesh(), FName("BackSocket"), this, this);

	}
	else
	{
		ActiveWeapon = EquippedWeapon1;
		EquippedWeapon1->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		EquippedWeapon2->Equip(GetMesh(), FName("BackSocket"), this, this);
	}
	UpdateWeaponHUD();
}

void APlayerCharacter::StartAttackTimer()
{
	GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::Attack, EquippedWeapon1->GetRateOfFireSeconds(), true, 0.f);
}

void APlayerCharacter::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void APlayerCharacter::UpdateWeaponHUD()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			if (UPlayerOverlay* PlayerOverlay = PlayerHUD->GetPlayerOverlay())
			{
				PlayerOverlay->SetAmmoCount(ActiveWeapon);
			}
		}
	}
}
