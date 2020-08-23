// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Weapons/Inventory.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    WeaponMesh->SetOnlyOwnerSee(true);
    WeaponMesh->bCastDynamicShadow = false;
    WeaponMesh->CastShadow = false;
    WeaponMesh->SetupAttachment(RootComponent);

    MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
    MuzzleLocation->SetupAttachment(WeaponMesh);

    GunOffset = FVector(100.f, 0.0f, 10.f);
}

void AWeaponBase::Fire(FRotator ControlRotation, UAnimInstance* AnimInstance, UInventory* Inventory)
{
    if (ProjectileClass != nullptr)
    {
        UWorld* const World = GetWorld();

        if (World != nullptr)
        {
            const FVector SpawnLocation = ((MuzzleLocation != nullptr)
                                               ? MuzzleLocation->GetComponentLocation()
                                               : GetActorLocation()) + ControlRotation.RotateVector(GunOffset);

            FActorSpawnParameters SpawnParameters;
            SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
            World->SpawnActor<APacktMasteringProjectile>(ProjectileClass, SpawnLocation, ControlRotation, SpawnParameters);
        }
    }

    if (FireSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
    }

    if (FireAnimation != nullptr)
    {
        if (AnimInstance != nullptr)
        {
            AnimInstance->Montage_Play(FireAnimation, 1.f);
        }
    }

    Inventory->ChangeAmmo(GetClass(), -1);
}
