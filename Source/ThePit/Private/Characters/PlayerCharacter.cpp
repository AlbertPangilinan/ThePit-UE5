// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

// Core
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Interfaces
#include "Interfaces/InteractInterface.h"

// Enhanced Input
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Enums
#include "Characters/CharacterEnums.h"
#include "Items/Weapons/WeaponEnums.h"

// Kismet
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 75.f);
	CameraBoom->TargetArmLength = 250.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("View Camera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Interact
	GetLineOfSightActor();
	UpdateInteractHUD();

	// Weapon Aim
	CalculateSpreadMultiplier();
	if (PlayerCombatState != EPlayerCombatState::EPCS_SwitchingWeapons && ActiveWeapon && ActiveWeapon->GetCurrentAmmoCount() <= 0 && ActiveWeapon->GetReserveAmmoCount() > 0) ReloadWeapon();
	AimZ = GetCameraRotation().Z;

	// ADS/Stance Toggle Interp
	if (CameraBoom->SocketOffset.Z != TargetCameraPosition)
	{
		CameraBoom->SocketOffset.Z = UKismetMathLibrary::FInterpTo_Constant(CameraBoom->SocketOffset.Z, TargetCameraPosition, DeltaTime, 500.f);
	}
	if (CameraBoom->TargetArmLength != TargetCameraZoom)
	{
		CameraBoom->TargetArmLength = UKismetMathLibrary::FInterpTo_Constant(CameraBoom->TargetArmLength, TargetCameraZoom, DeltaTime, 750.f);
	}
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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &APlayerCharacter::Attack);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &APlayerCharacter::ClearAttackTimer);
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleADS);
		EnhancedInputComponent->BindAction(CycleFireModeAction, ETriggerEvent::Started, this, &APlayerCharacter::CycleFireMode);
		EnhancedInputComponent->BindAction(SwitchWeaponAction, ETriggerEvent::Triggered, this, &APlayerCharacter::SwitchWeapon);
		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::ReloadWeapon);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &APlayerCharacter::Interact);
	}
}

FVector APlayerCharacter::GetCameraLocation()
{
	return ViewCamera->GetComponentLocation();
}

FVector APlayerCharacter::GetCameraRotation()
{
	return ViewCamera->GetComponentRotation().GetNormalized().Vector();
}

FHitResult APlayerCharacter::LineOfSightLineTrace(TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes, TArray<AActor*> ActorsToIgnore)
{
	FHitResult LineOfSightResult;

	const FVector LineOfSightStart = GetCameraLocation() + GetCameraRotation();
	const FVector LineOfSightEnd = GetCameraLocation() + GetCameraRotation() * MaxRange;

	UKismetSystemLibrary::LineTraceSingleForObjects(this, LineOfSightStart, LineOfSightEnd, ObjectTypes, false, ActorsToIgnore, EDrawDebugTrace::None, LineOfSightResult, true, FColor::Blue);
	return LineOfSightResult;
}

double APlayerCharacter::GetGroundSpeed()
{
	return UKismetMathLibrary::VSizeXY(GetCharacterMovement()->Velocity);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateWeaponHUD();
	InitializeCameraSettings();

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
	switch (PlayerStance)
	{
		case EPlayerStance::EPS_Crouching:
			PlayerStance = EPlayerStance::EPS_Standing;
			TargetCameraPosition = 75.f;
			/*if (UCapsuleComponent* PlayerCapsule = Cast<UCapsuleComponent>(GetRootComponent()))
			{
				PlayerCapsule->SetCapsuleHalfHeight(90.f);
				PlayerCapsule->SetCapsuleRadius(35.f);
			}*/
			break;
		case EPlayerStance::EPS_Standing:
			PlayerStance = EPlayerStance::EPS_Crouching;
			TargetCameraPosition = 25.f;
			/*if (UCapsuleComponent* PlayerCapsule = Cast<UCapsuleComponent>(GetRootComponent()))
			{
				PlayerCapsule->SetCapsuleHalfHeight(60.f);
				PlayerCapsule->SetCapsuleRadius(60.f);
			}*/
			break;
		default:
			break;
	}
}

void APlayerCharacter::Attack()
{
	if (ActiveWeapon == nullptr || PlayerCombatState == EPlayerCombatState::EPCS_SwitchingWeapons || ActiveWeapon->GetCurrentAmmoCount() <= 0) return;
	CombatMontage->BlendIn = 0.05f;
	CombatMontage->BlendOut = 0.05f;
	if (PlayerCombatState == EPlayerCombatState::EPCS_Reloading) StopAnimMontage();

	PlayerCombatState = EPlayerCombatState::EPCS_Firing;
	EWeaponFireMode CurrentFireMode = ActiveWeapon->GetFireMode();

	switch (CurrentFireMode)
	{
	case EWeaponFireMode::EWFM_FullAuto:
		AttackFullAuto();
		break;
	case EWeaponFireMode::EWFM_SemiAuto:
		AttackSemiAuto();
		break;
	default:
		break;
	}
}

void APlayerCharacter::AttackFullAuto()
{
	StartAttackTimer();
}

void APlayerCharacter::AttackSemiAuto()
{
	if (ActiveWeapon->GetCurrentAmmoCount() <= 0 || PlayerCombatState == EPlayerCombatState::EPCS_Reloading) return;
	ActiveWeapon->Fire();
	//PlayMontageSection(CombatMontage, FName("Fire"));
	UpdateWeaponHUD();
}

void APlayerCharacter::ToggleADS()
{
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

	UpdateWeaponHUD();
}

void APlayerCharacter::SwitchWeapon()
{
	if (ReserveWeapon == nullptr) return;
	ClearAttackTimer();
	StopAnimMontage();
	PlayerCombatState = EPlayerCombatState::EPCS_SwitchingWeapons;
	SwitchWeaponSockets();
	CombatMontage->BlendIn = 0.05f;
	CombatMontage->BlendOut= 0.05f;
	PlayMontageSection(CombatMontage, FName("Equip"));
	UpdateWeaponHUD();
}

void APlayerCharacter::StartAttackTimer()
{
	GetWorldTimerManager().SetTimer(AttackTimer, this, &APlayerCharacter::AttackSemiAuto, ActiveWeapon->GetRateOfFireSeconds(), true, 0.f);
}

void APlayerCharacter::ClearAttackTimer()
{
	if (PlayerCombatState != EPlayerCombatState::EPCS_Reloading)
	{
		CombatMontage->BlendIn = 0.25f;
		CombatMontage->BlendOut = 0.25f;
		PlayerCombatState = EPlayerCombatState::EPCS_Aiming;
		GetWorldTimerManager().ClearTimer(AttackTimer);
	}
}

void APlayerCharacter::Interact()
{
	if (CanInteract())
	{
		IInteractInterface* InteractableActor = Cast<IInteractInterface>(LineOfSightActor);
		InteractableActor->Interact();
	}
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

void APlayerCharacter::CalculateSpreadMultiplier()
{
	float CurrentSpreadMultiplier = 1.f;
	if (GetGroundSpeed() > 0.0) CurrentSpreadMultiplier *= 1.25;
	if (GetPlayerStance() == EPlayerStance::EPS_Crouching) CurrentSpreadMultiplier *= 0.75;
	if (GetPlayerAimState() == EPlayerAimState::EPAS_ADS) CurrentSpreadMultiplier *= 0.5;
	SpreadMultiplier = CurrentSpreadMultiplier;
	UpdateWeaponHUD();
}

void APlayerCharacter::EquipWeapon(AWeapon* Weapon)
{
	if (ReserveWeapon != nullptr) // 2 weapons
	{
		ActiveWeapon->Unequip();
		ActiveWeapon = nullptr;
	}
	else
	{
		if (ActiveWeapon != nullptr) // 1 weapon
		{
			ActiveWeapon->Unequip();
			ReserveWeapon = ActiveWeapon;
			ActiveWeapon->Equip(FName("BackSocket"));
			ActiveWeapon = nullptr;
		}
	}

	ActiveWeapon = Weapon;
	Weapon->Equip(FName("RightHandSocket"));
}

void APlayerCharacter::ReloadWeapon()
{
	if (ActiveWeapon->GetCurrentAmmoCount() >= ActiveWeapon->GetMagazineSize() || ActiveWeapon->GetReserveAmmoCount() <= 0 || PlayerCombatState == EPlayerCombatState::EPCS_Reloading) return;
	ClearAttackTimer();
	PlayerCombatState = EPlayerCombatState::EPCS_Reloading;
	PlayMontageSection(CombatMontage, FName("Reload"));
}

void APlayerCharacter::StartWeaponSwitch()
{
	CombatMontage->BlendIn = 0.25f;
	CombatMontage->BlendOut = 0.25f;
}

void APlayerCharacter::EndWeaponSwitch()
{
	PlayerCombatState = EPlayerCombatState::EPCS_Aiming;
}

void APlayerCharacter::StartReloadActiveWeapon()
{
	if (ActiveWeapon == nullptr) return;
	if (ActiveWeapon->GetCurrentAmmoCount() < ActiveWeapon->GetMagazineSize())
	{
		ClearAttackTimer();
		PlayerCombatState = EPlayerCombatState::EPCS_Reloading;
	}
}

void APlayerCharacter::EndReloadActiveWeapon()
{
	if (ActiveWeapon == nullptr) return;
	if (ActiveWeapon->GetCurrentAmmoCount() < ActiveWeapon->GetMagazineSize())
	{
		ActiveWeapon->Reload();
		UpdateWeaponHUD();
		PlayerCombatState = EPlayerCombatState::EPCS_Aiming;
	}
}

void APlayerCharacter::InitializeCameraSettings()
{
	TargetCameraPosition = CameraBoom->SocketOffset.Z;
	TargetCameraZoom = CameraBoom->TargetArmLength;
}

void APlayerCharacter::UpdateWeaponHUD()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			if (UPlayerOverlay* PlayerOverlay = PlayerHUD->GetPlayerOverlay())
			{
				if (ActiveWeapon && SpreadMultiplier)
				{
					PlayerOverlay->SetCrosshairSpread(ActiveWeapon, SpreadMultiplier);
					PlayerOverlay->SetCurrentAmmoCount(ActiveWeapon);
					PlayerOverlay->SetWeaponName(ActiveWeapon);
					PlayerOverlay->SetFireMode(ActiveWeapon);
				}
				else
				{
					PlayerOverlay->ClearCrosshairSpread();
					PlayerOverlay->ClearCurrentAmmoCount();
					PlayerOverlay->ClearWeaponName();
					PlayerOverlay->ClearFireMode();
				}
			}
		}
	}
}

void APlayerCharacter::SwitchWeaponSockets()
{
	ActiveWeapon->Unequip();
	ReserveWeapon->Unequip();
	ActiveWeapon->Equip(FName("BackSocket"));
	ReserveWeapon->Equip(FName("RightHandSocket"));

	AWeapon* TempWeapon = ReserveWeapon;
	ReserveWeapon = ActiveWeapon;
	ActiveWeapon = TempWeapon;
}

void APlayerCharacter::UpdateInteractHUD()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD()))
		{
			if (UPlayerOverlay* PlayerOverlay = PlayerHUD->GetPlayerOverlay())
			{
				if (CanInteract())
				{
					IInteractInterface* InteractableActor = Cast<IInteractInterface>(LineOfSightActor);
					PlayerOverlay->SetInteractAction(InteractableActor);
				}
				else
				{
					PlayerOverlay->ClearInteractAction();
				}
			}
		}
	}
}

void APlayerCharacter::GetLineOfSightActor()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());
	ActorsToIgnore.Add(ActiveWeapon);
	ActorsToIgnore.Add(ReserveWeapon);

	FHitResult LineOfSightResult = LineOfSightLineTrace(ObjectTypes, ActorsToIgnore);
	if (LineOfSightResult.IsValidBlockingHit())
	{
		LineOfSightActor = LineOfSightResult.GetActor();
	}
	else
	{
		LineOfSightActor = nullptr;
	}
}

bool APlayerCharacter::CanInteract()
{
	return ((OverlappingActors.Contains(LineOfSightActor)) && (Cast<IInteractInterface>(LineOfSightActor)));
}
