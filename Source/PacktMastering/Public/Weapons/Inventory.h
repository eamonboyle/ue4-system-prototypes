// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/WeaponBase.h"
#include "Inventory.generated.h"

USTRUCT()
struct FWeaponProperties
{
    GENERATED_BODY()

public:
    UPROPERTY()
    TSubclassOf<class AWeaponBase> WeaponClass;
    UPROPERTY()
    int WeaponPower;
    UPROPERTY()
    int Ammo;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PACKTMASTERING_API UInventory : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UInventory();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AWeaponBase> DefaultWeapon;

    void SelectBestWeapon();

    void SelectWeapon(TSubclassOf<AWeaponBase> Weapon);

    void AddWeapon(TSubclassOf<AWeaponBase> Weapon, int Ammunition, uint8 WeaponPower);

    FORCEINLINE TSubclassOf<AWeaponBase> GetCurrentWeapon() const { return CurrentWeapon; }

    void ChangeAmmo(TSubclassOf<AWeaponBase> Weapon, const int ChangeAmount);

    int FindCurrentWeaponIndex() const;

    void SelectNextWeapon();
    void SelectPreviousWeapon();

protected:
    TArray<FWeaponProperties> WeaponsArray;
    TSubclassOf<AWeaponBase> CurrentWeapon;
    int CurrentWeaponPower;
    class APacktMasteringCharacter* MyOwner;
};
