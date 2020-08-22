// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PACKTMASTERING_API AMainHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMainHUD();

    virtual void BeginPlay() override;

    virtual void DrawHUD() override;

    virtual void InitializeInventory(class UInventory* PlayerInventory);

    virtual void ToggleMainMenu();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UTexture2D* CrosshairTex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> InventoryClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UUserWidget> MainMenuClass;

    class UInventoryDisplay* InventoryHUD;
    class UMainMenuWidget* MainMenu;
    class UInventory* Inventory;

    bool bNeedsInventoryInit = true;
};
