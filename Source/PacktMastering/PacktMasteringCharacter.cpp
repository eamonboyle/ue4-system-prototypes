// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacktMasteringCharacter.h"
#include "PacktMasteringProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Weapons/Inventory.h"
#include "Weapons/WeaponBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// APacktMasteringCharacter

APacktMasteringCharacter::APacktMasteringCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	Inventory = CreateDefaultSubobject<UInventory>(TEXT("Inventory"));
}

void APacktMasteringCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (Inventory != nullptr)
	{
		Inventory->SelectBestWeapon();
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void APacktMasteringCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APacktMasteringCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APacktMasteringCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APacktMasteringCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APacktMasteringCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APacktMasteringCharacter::LookUpAtRate);

	// inventory
	PlayerInputComponent->BindAction("InventoryUp", IE_Pressed, this, &APacktMasteringCharacter::SelectNextWeapon);
	PlayerInputComponent->BindAction("InventoryDown", IE_Pressed, this, &APacktMasteringCharacter::SelectPreviousWeapon);
}

void APacktMasteringCharacter::EquipWeapon(TSubclassOf<AWeaponBase> Weapon)
{
	UWorld* World = GetWorld();

	if (World == nullptr) return;

	if (EquippedWeaponActor != nullptr)
	{
		World->DestroyActor(EquippedWeaponActor);
	}

	const FRotator SpawnRotation = GetActorRotation();
	const FVector SpawnLocation = GetActorLocation();
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ActorSpawnParameters.Owner = this;
	EquippedWeaponActor = Cast<AWeaponBase>(World->SpawnActor(Weapon, &SpawnLocation, &SpawnRotation, ActorSpawnParameters));

	if (EquippedWeaponActor != nullptr)
	{
		EquippedWeaponActor->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

void APacktMasteringCharacter::OnFire()
{
	if (GetEquippedWeapon() != nullptr)
	{
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		GetEquippedWeapon()->Fire(GetControlRotation(), AnimInstance, GetInventory());
	}
}

void APacktMasteringCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APacktMasteringCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APacktMasteringCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APacktMasteringCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void APacktMasteringCharacter::SelectNextWeapon()
{
	Inventory->SelectNextWeapon();
}

void APacktMasteringCharacter::SelectPreviousWeapon()
{
	Inventory->SelectPreviousWeapon();
}
