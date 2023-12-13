// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"

// Enhanced Input
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Enums
#include "Characters/CharacterEnums.h"
#include "Items/Weapons/WeaponEnums.h"

// Kismet
#include "Kismet/GameplayStatics.h"

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
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleADS);
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchWeapon);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &APlayerCharacter::PlayReloadAnim);
	}
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

AWeapon* APlayerCharacter::GetActiveWeapon()
{
	if (ActiveWeapon) return ActiveWeapon;
	return nullptr;
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	MovementVector = Value.Get<FVector2D>();
	double ForwardMovement = MovementVector.Y;
	double RightMovement = MovementVector.X;

	if (PlayerAimState == EPlayerAimState::EPAS_ADS)
	{
		ForwardMovement *= 0.5;
		RightMovement *= 0.5;
	}

	if (PlayerStance == EPlayerStance::EPS_Crouching)
	{
		ForwardMovement *= 0.5;
		RightMovement *= 0.5;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(ForwardDirection, ForwardMovement);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, RightMovement);
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
	FVector TargetCameraPosition;

	switch (PlayerStance)
	{
		case EPlayerStance::EPS_Crouching:
			PlayerStance = EPlayerStance::EPS_Standing;
			TargetCameraPosition = FVector(0.f, 50.f, 75.f);
			/*if (UCapsuleComponent* PlayerCapsule = Cast<UCapsuleComponent>(GetRootComponent()))
			{
				PlayerCapsule->SetCapsuleHalfHeight(90.f);
				PlayerCapsule->SetCapsuleRadius(35.f);
			}*/
			break;
		case EPlayerStance::EPS_Standing:
			PlayerStance = EPlayerStance::EPS_Crouching;
			TargetCameraPosition = FVector(0.f, 50.f, 25.f);
			/*if (UCapsuleComponent* PlayerCapsule = Cast<UCapsuleComponent>(GetRootComponent()))
			{
				PlayerCapsule->SetCapsuleHalfHeight(60.f);
				PlayerCapsule->SetCapsuleRadius(60.f);
			}*/
			break;
		default:
			break;
	}

	while (!FMath::IsNearlyEqual(CameraBoom->SocketOffset.Z, TargetCameraPosition.Z))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%d"), CameraBoom->SocketOffset.Z);
		CameraBoom->SocketOffset = FMath::VInterpConstantTo(CameraBoom->SocketOffset, TargetCameraPosition, UGameplayStatics::GetWorldDeltaSeconds(this), 1.f);
	}
}

void APlayerCharacter::Attack()
{
	if (ActiveWeapon == nullptr || ActiveWeapon->GetAmmoCount() <= 0 || PlayerCombatState == EPlayerCombatState::EPCS_Reloading) return;
	PlayerCombatState = EPlayerCombatState::EPCS_Firing;
	ActiveWeapon->Fire();
	UpdateWeaponHUD();

	/*switch (PlayerStance)
	{
	case EPlayerStance::EPS_Standing:
		PlayMontageSection(RunCombatMontage, FName("Fire"));
		break;
	case EPlayerStance::EPS_Crouching:
		PlayMontageSection(CrouchCombatMontage, FName("Fire"));
		break;
	default:
		break;
	}*/
}

void APlayerCharacter::ToggleADS()
{
	float TargetCameraZoom;

	switch (PlayerAimState)
	{
		case EPlayerAimState::EPAS_Hipfire:
			PlayerAimState = EPlayerAimState::EPAS_ADS;
			TargetCameraZoom = 100.f;
			break;
		case EPlayerAimState::EPAS_ADS:
			PlayerAimState = EPlayerAimState::EPAS_Hipfire;
			TargetCameraZoom = 250.f;
			break;
		default:
			break;
	}

	while (!FMath::IsNearlyEqual(CameraBoom->TargetArmLength, TargetCameraZoom))
	{
		CameraBoom->TargetArmLength = FMath::FInterpConstantTo(CameraBoom->TargetArmLength, TargetCameraZoom, UGameplayStatics::GetWorldDeltaSeconds(this), 1.f);
	}
}

void APlayerCharacter::CycleFireMode()
{
	if (ActiveWeapon == nullptr) return;

	EWeaponFireMode CurrentFireMode = ActiveWeapon->GetFireMode();

	switch (CurrentFireMode)
	{
	case EWeaponFireMode::EWFM_FullAuto:
		ActiveWeapon->SetFireMode(EWeaponFireMode::EWFM_SemiAuto);
		break;
	case EWeaponFireMode::EWFM_SemiAuto:
		ActiveWeapon->SetFireMode(EWeaponFireMode::EWFM_FullAuto);
		break;
	default:
		break;
	}
}

void APlayerCharacter::SwitchWeapon()
{
	ClearAttackTimer();

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
	GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::Attack, ActiveWeapon->GetRateOfFireSeconds(), true, 0.f);
}

void APlayerCharacter::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

void APlayerCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
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

void APlayerCharacter::PlayReloadAnim()
{
	if (ActiveWeapon->GetAmmoCount() >= ActiveWeapon->GetMagazineSize() || PlayerCombatState == EPlayerCombatState::EPCS_Reloading) return;
	PlayerCombatState = EPlayerCombatState::EPCS_Reloading;
	switch (PlayerStance)
	{
	case EPlayerStance::EPS_Standing:
		PlayMontageSection(RunCombatMontage, FName("Reload"));
		break;
	case EPlayerStance::EPS_Crouching:
		PlayMontageSection(CrouchCombatMontage, FName("Reload"));
		break;
	default:
		break;
	}
}

void APlayerCharacter::ReloadActiveWeapon()
{
	if (ActiveWeapon == nullptr) return;
	if (ActiveWeapon->GetAmmoCount() < ActiveWeapon->GetMagazineSize())
	{
		ActiveWeapon->Reload();
		UpdateWeaponHUD();
		PlayerCombatState = EPlayerCombatState::EPCS_Aiming;
	}
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
				PlayerOverlay->SetWeaponName(ActiveWeapon);
				PlayerOverlay->SetFireMode(ActiveWeapon);
			}
		}
	}
}
