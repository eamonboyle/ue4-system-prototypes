// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponPickup.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponBase.h"
#include "Inventory.generated.h"

USTRUCT(BlueprintType)
struct FWeaponProperties
{
    GENERATED_USTRUCT_BODY()

public:

    FWeaponProperties() { };

    FWeaponProperties(TSubclassOf<class AWeaponBase> Class, UTexture2D* Icon, int Power, int AmmoCount) :
        WeaponClass(Class),
        InventoryIcon(Icon),
        WeaponPower(Power),
        Ammo(AmmoCount)
    { }

    bool operator==(const FWeaponProperties& Other) const
    {
        return Other.WeaponClass == WeaponClass;
    }

    UPROPERTY(BlueprintReadOnly)
    TSubclassOf<class AWeaponBase> WeaponClass;

    UPROPERTY(BlueprintReadOnly)
    class UTexture2D* InventoryIcon;

    UPROPERTY(BlueprintReadOnly)
    int WeaponPower;

    UPROPERTY(BlueprintReadOnly)
    int Ammo;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACKTMASTERING_API UInventory : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventory();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AWeaponPickup> DefaultWeaponPickup;
    
    virtual void AddDefaultWeapon();

    void SelectBestWeapon();

    void SelectWeapon(FWeaponProperties Weapon);

    void AddWeapon(const FWeaponProperties &Properties);

    FORCEINLINE TSubclassOf<AWeaponBase> GetCurrentWeapon() const { return CurrentWeapon; }

    void ChangeAmmo(TSubclassOf<AWeaponBase> Weapon, const int ChangeAmount);

    int FindCurrentWeaponIndex() const;

    void SelectNextWeapon();
    void SelectPreviousWeapon();

    DECLARE_EVENT_OneParam(UInventory, FSelectedWeaponChanged, FWeaponProperties);
    FSelectedWeaponChanged OnSelectedWeaponChanged;

    DECLARE_EVENT_OneParam(UInventory, FWeaponAdded, FWeaponProperties);
    FSelectedWeaponChanged OnWeaponAdded;

    DECLARE_EVENT_OneParam(UInventory, FWeaponRemoved, FWeaponProperties);
    FSelectedWeaponChanged OnWeaponRemoved;

protected:
    TArray<FWeaponProperties> WeaponsArray;
    TSubclassOf<AWeaponBase> CurrentWeapon;
    int CurrentWeaponPower;
    class APacktMasteringCharacter* MyOwner;
};
