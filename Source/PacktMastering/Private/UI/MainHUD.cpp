// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Canvas.h"
#include "UI/InventoryDisplay.h"

AMainHUD::AMainHUD()
{
}

void AMainHUD::BeginPlay()
{
    UE_LOG(LogTemp, Warning, TEXT("BEGINPLAY ON MAINHUD"));
       
    if (InventoryClass != nullptr)
    {        
        InventoryHUD = CreateWidget<UInventoryDisplay>(GetOwningPlayerController(), InventoryClass);
        checkSlow(InventoryHUD != nullptr);

        InventoryHUD->AddToViewport();
    }

    // if (MainMenuClass != nullptr)
    // {
    //     
    // }
}

void AMainHUD::DrawHUD()
{
    if (bNeedsInventoryInit && Inventory != nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("FOUND INVENTORY"));
        if (InventoryHUD != nullptr)
        {
            InventoryHUD->Init(Inventory);
        }

        Inventory->AddDefaultWeapon();
        Inventory->SelectBestWeapon();

        bNeedsInventoryInit = false;
    }

    Super::DrawHUD();

    if (CrosshairTex != nullptr)
    {
        const FVector2D Center(Canvas->ClipX * .5f, Canvas->ClipY * .5f);

        const FVector2D CrosshairDrawPosition((Center.X), (Center.Y + 20.f));

        FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
        TileItem.BlendMode = SE_BLEND_Translucent;
        Canvas->DrawItem(TileItem);
    }
}

void AMainHUD::InitializeInventory(UInventory* PlayerInventory)
{
    // TODO: HOW TO CALL THIS
    Inventory = PlayerInventory;

    if (InventoryHUD != nullptr)
    {
        InventoryHUD->Init(Inventory);
    }
}

void AMainHUD::ToggleMainMenu()
{
}
