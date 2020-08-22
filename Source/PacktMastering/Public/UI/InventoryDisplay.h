// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapons/Inventory.h"

#include "InventoryDisplay.generated.h"

/**
 * 
 */
UCLASS()
class PACKTMASTERING_API UInventoryDisplay : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void Init(class UInventory* Inventory);

    UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
    void WeaponSelected(FWeaponProperties Weapon);

    UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
    void WeaponAdded(FWeaponProperties Weapon);

    UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
    void WeaponRemoved(FWeaponProperties Weapon);
};
