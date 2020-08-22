// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Inventory.h"
#include "PacktMastering/PacktMasteringCharacter.h"

#define UNLIMITED_AMMO -1

UInventory::UInventory()
{
    CurrentWeaponPower = -1;
    MyOwner = Cast<APacktMasteringCharacter>(GetOwner());
    check(GetOwner() == nullptr || MyOwner != nullptr);
}

void UInventory::BeginPlay()
{
    Super::BeginPlay();

    if (DefaultWeapon != nullptr)
    {
        AddWeapon(DefaultWeapon, UNLIMITED_AMMO, 0);
    }
}

void UInventory::SelectBestWeapon()
{
    UE_LOG(LogTemp, Warning, TEXT("SELECT BEST WEAPON"));

    for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
    {
        UE_LOG(LogTemp, Warning, TEXT("Weapon Power: %d"), WeaponIt->WeaponPower);

        // if (WeaponIt->WeaponPower > CurrentWeaponPower)
        // {
        SelectWeapon(*WeaponIt->WeaponClass);
        // break;
        // }
    }
}

void UInventory::SelectWeapon(TSubclassOf<AWeaponBase> Weapon)
{
    MyOwner->EquipWeapon(Weapon);
    CurrentWeapon = Weapon;
}

void UInventory::AddWeapon(TSubclassOf<AWeaponBase> Weapon, int Ammunition, uint8 WeaponPower)
{
    for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
    {
        FWeaponProperties& CurrentProps = *WeaponIt;

        if (CurrentProps.WeaponClass == Weapon)
        {
            checkSlow(Ammunition >= 0);
            CurrentProps.Ammo += Ammunition;
            return;
        }
    }

    FWeaponProperties WeaponProps;
    WeaponProps.WeaponClass = Weapon;
    WeaponProps.WeaponPower = WeaponPower;
    WeaponProps.Ammo = Ammunition;
    WeaponsArray.Add(WeaponProps);
}

void UInventory::ChangeAmmo(TSubclassOf<AWeaponBase> Weapon, const int ChangeAmount)
{
    for (auto WeaponIt = WeaponsArray.CreateIterator(); WeaponIt; ++WeaponIt)
    {
        FWeaponProperties CurrentProps = *WeaponIt;

        if (CurrentProps.WeaponClass == Weapon)
        {
            if (CurrentProps.Ammo == UNLIMITED_AMMO)
            {
                return;
            }

            CurrentProps.Ammo = FMath::Clamp(CurrentProps.Ammo + ChangeAmount, 0, 999);

            if (CurrentProps.Ammo == 0)
            {
                CurrentWeaponPower = -1;
                SelectBestWeapon();
                return;
            }
        }
    }
}

int UInventory::FindCurrentWeaponIndex() const
{
    int CurrentIndex = 0;
    for (auto WeaponIt = WeaponsArray.CreateConstIterator(); WeaponIt; ++WeaponIt, ++CurrentIndex)
    {
        const FWeaponProperties& CurrentProps = *WeaponIt;
        if (CurrentProps.WeaponClass == CurrentWeapon) break;
    }
    checkSlow(currentIndex < WeaponsArray.Num());
    return CurrentIndex;
}

void UInventory::SelectNextWeapon()
{
    int CurrentIndex = FindCurrentWeaponIndex();

    if (CurrentIndex == WeaponsArray.Num() - 1)
    {
        SelectWeapon(WeaponsArray[0].WeaponClass);
    }
    else
    {
        SelectWeapon(WeaponsArray[CurrentIndex + 1].WeaponClass);
    }
}

void UInventory::SelectPreviousWeapon()
{
    int CurrentIndex = FindCurrentWeaponIndex();

    if (CurrentIndex > 0)
    {
        SelectWeapon(WeaponsArray[CurrentIndex - 1].WeaponClass);
    }
    else
    {
        SelectWeapon(WeaponsArray[WeaponsArray.Num() - 1].WeaponClass);
    }
}
