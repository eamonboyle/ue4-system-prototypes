// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Inventory.h"
#include "PacktMastering/PacktMasteringCharacter.h"
#include "Weapons/WeaponPickup.h"

#define UNLIMITED_AMMO -1

UInventory::UInventory()
{
    CurrentWeaponPower = -1;
    MyOwner = Cast<APacktMasteringCharacter>(GetOwner());
    check(GetOwner() == nullptr || MyOwner != nullptr);
}

void UInventory::AddDefaultWeapon()
{
    if (DefaultWeaponPickup != nullptr)
    {
        FActorSpawnParameters ActorSpawnParameters;
        ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AWeaponPickup* DefaultPickup = GetWorld()->SpawnActor<AWeaponPickup>(
            DefaultWeaponPickup, FVector(0.f), FRotator(0.f), ActorSpawnParameters);
        DefaultPickup->HavePlayerPickup(MyOwner);
    }
}

void UInventory::SelectBestWeapon()
{
    int HighestWeaponPower = CurrentWeaponPower;
    FWeaponProperties BestWeapon;

    for (auto WeaponIt = WeaponsArray.CreateConstIterator(); WeaponIt; ++WeaponIt)
    {
        const FWeaponProperties& CurrentProps = *WeaponIt;

        // skip any weapons that don't have ammo, ammo of -1 is a special case meaning it has unlimited ammo
        if (CurrentProps.Ammo == 0)
            continue;

        if (CurrentProps.WeaponPower > HighestWeaponPower)
        {
            HighestWeaponPower = CurrentProps.WeaponPower;
            CurrentWeaponPower = HighestWeaponPower;
            BestWeapon = CurrentProps;
        }
    }

    if (BestWeapon.WeaponClass != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("BEST WEAPON: %s"), *BestWeapon.WeaponClass->GetName());
        SelectWeapon(BestWeapon);
    }
}

void UInventory::SelectWeapon(FWeaponProperties Weapon)
{
    checkf(Weapon.WeaponClass != nullptr, TEXT("Weapon equip attempted with null class!"));

    UE_LOG(LogTemp, Warning, TEXT("BROADCAST NOW"));
    OnSelectedWeaponChanged.Broadcast(Weapon);
    MyOwner->EquipWeapon(Weapon.WeaponClass);
    CurrentWeapon = Weapon.WeaponClass;
}

void UInventory::AddWeapon(const FWeaponProperties& Properties)
{
    int FoundIndex = WeaponsArray.Find(Properties);
    if (FoundIndex != INDEX_NONE)
    {
        FWeaponProperties& CurrentProps = WeaponsArray[FoundIndex];
        checkSlow(Properties.Ammo >= || Properties.Ammo == UNLIMITED_AMMO);
        CurrentProps.Ammo += Properties.Ammo;
    }
    else
    {
        WeaponsArray.Add(Properties);
        OnWeaponAdded.Broadcast(Properties);
    }
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
                OnWeaponRemoved.Broadcast(CurrentProps);
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
        SelectWeapon(WeaponsArray[0]);
    }
    else
    {
        SelectWeapon(WeaponsArray[CurrentIndex + 1]);
    }
}

void UInventory::SelectPreviousWeapon()
{
    int CurrentIndex = FindCurrentWeaponIndex();

    if (CurrentIndex > 0)
    {
        SelectWeapon(WeaponsArray[CurrentIndex - 1]);
    }
    else
    {
        SelectWeapon(WeaponsArray[WeaponsArray.Num() - 1]);
    }
}
