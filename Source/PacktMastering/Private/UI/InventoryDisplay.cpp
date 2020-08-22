// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryDisplay.h"
#include "Weapons/Inventory.h"

void UInventoryDisplay::Init(UInventory* Inventory)
{
    UE_LOG(LogTemp, Warning, TEXT("Inventory Display Init()"));
    Inventory->OnSelectedWeaponChanged.AddUObject(this, &UInventoryDisplay::WeaponSelected);
    Inventory->OnWeaponAdded.AddUObject(this, &UInventoryDisplay::WeaponAdded);
    Inventory->OnWeaponRemoved.AddUObject(this, &UInventoryDisplay::WeaponRemoved);
}
