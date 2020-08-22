// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponPickup.h"
#include "Weapons/Inventory.h"
#include "PacktMastering/PacktMasteringCharacter.h"

AWeaponPickup::AWeaponPickup()
{
    PrimaryActorTick.bCanEverTick = true;

    RotationSpeed = 30.f;
    Ammunition = 10;
    WeaponPower = 1;
}

void AWeaponPickup::BeginPlay()
{
    Super::BeginPlay();
}

void AWeaponPickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    APacktMasteringCharacter* Player = Cast<APacktMasteringCharacter>(OtherActor);

    if (Player == nullptr) return;

    UInventory* Inventory = Player->GetInventory();
    Inventory->AddWeapon(WeaponClass, Ammunition, WeaponPower);
    Inventory->SelectBestWeapon();
    Destroy();
}

void AWeaponPickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FRotator RotationAmount(0.f, DeltaTime * RotationSpeed, 0.f);
    AddActorLocalRotation(RotationAmount);
}
