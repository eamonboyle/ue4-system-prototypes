// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"
#include "PacktMastering/PacktMasteringCharacter.h"
#include "Serialization/BufferArchive.h"
#include "UI/SavedActorInterface.h"
#include "Weapons/Inventory.h"

void UMainMenuWidget::LoadGame(FString SaveFile)
{
    FString OutPath = FPaths::ProjectSavedDir() + SaveFile;
    if (!FFileHelper::LoadFileToArray(BinaryData, *OutPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *(FString("Game Load Failed: ") + OutPath));
        return;
    }

    FMemoryReader FromBinary = FMemoryReader(BinaryData, true);
    FromBinary.Seek(0);

    FGameSavedData SaveGameData;
    FromBinary << SaveGameData;

    FromBinary.Flush();
    FromBinary.Close();

    FString MapName = SaveGameData.MapName.ToString();
    UWorld *World = GetWorld();

    FString CurrentMapName = World->GetMapName();

    CurrentMapName.Split("_", nullptr, &CurrentMapName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

    if (MapName == CurrentMapName)
    {
        World->ServerTravel("?Restart", true);
    }
    else
    {
        UGameplayStatics::OpenLevel(World, *MapName, true);
    }
}

void UMainMenuWidget::SaveGame()
{
    UE_LOG(LogTemp, Warning, TEXT("SAVE THE GAME"));

    FGameSavedData SaveGameData;

    SaveGameData.Timestamp = FDateTime::Now();

    UWorld* World = GetWorld();
    checkSlow(World != nullptr);

    FString MapName = World->GetMapName();

    MapName.Split("_", nullptr, &MapName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

    SaveGameData.MapName = *MapName;

    TArray<AActor*> Actors;
    UGameplayStatics::GetAllActorsWithInterface(GetWorld(), USavedActorInterface::StaticClass(), Actors);

    UE_LOG(LogTemp, Warning, TEXT("ACTORS TO SAVE: %d"), Actors.Num());

    TArray<FActorSavedData> SavedActors;
    for (auto Actor : Actors)
    {
        FActorSavedData ActorRecord;
        ActorRecord.MyName = FName(*Actor->GetName());
        ActorRecord.MyClass = Actor->GetClass()->GetPathName();
        ActorRecord.MyTransform = Actor->GetTransform();
        ActorRecord.MyVelocity = Actor->GetVelocity();

        FMemoryWriter MemoryWriter(ActorRecord.MyData, true);
        FSaveGameArchive Ar(MemoryWriter);
        APacktMasteringCharacter* Mast = Cast<APacktMasteringCharacter>(Actor);

        ISavedActorInterface::Execute_ActorSaved(Actor);

        Actor->Serialize(Ar);

        if (Mast != nullptr)
        {
            UInventory* Inv = Mast->GetInventory();
            SaveGameData.InventoryData.CurrentWeapon = Inv->GetCurrentWeapon()->GetPathName();
            SaveGameData.InventoryData.CurrentWeaponPower = Inv->GetCurrentWeaponPower();

            for (FWeaponProperties Weapon : Inv->GetWeaponsArray())
            {
                FInventoryItemData Data;
                Data.WeaponClass = Weapon.WeaponClass->GetPathName();
                Data.WeaponPower = Weapon.WeaponPower;
                Data.Ammo = Weapon.Ammo;
                Data.TextureClass = Weapon.InventoryIcon->GetPathName();

                SaveGameData.InventoryData.WeaponsArray.Add(Data);
            }
        }

        SavedActors.Add(ActorRecord);
    }

    FBufferArchive SaveData;

    SaveGameData.SavedActors = SavedActors;

    SaveData << SaveGameData;

    FString OutPath = FPaths::ProjectSavedDir() + SaveGameData.Timestamp.ToString() + TEXT(".sav");

    FFileHelper::SaveArrayToFile(SaveData, *OutPath);

    SaveData.FlushCache();
    SaveData.Empty();

    APlayerController* PlayerController = World->GetFirstPlayerController();

    if (PlayerController)
    {
        PlayerController->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);
        UGameplayStatics::SetGamePaused(this, false);
    }

    Close();
}

void UMainMenuWidget::Open()
{
    checkSlow(GetVisibility() == ESlateVisibility::Hidden);
    SetVisibility(ESlateVisibility::Visible);

    UWorld* World = GetWorld();
    if (World != nullptr)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            FInputModeUIOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            UGameplayStatics::SetGamePaused(this, true);
        }
    }
}

void UMainMenuWidget::Close()
{
    checkSlow(GetVisibility() == ESlateVisibility::Visible);
    SetVisibility(ESlateVisibility::Hidden);

    UWorld* World = GetWorld();
    if (World != nullptr)
    {
        APlayerController* PlayerController = World->GetFirstPlayerController();

        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = false;
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);
            UGameplayStatics::SetGamePaused(this, false);
        }
    }
}

void UMainMenuWidget::PopulateSaveFiles()
{
    SaveFileNames.Empty();
    FString Dir = FPaths::ProjectSavedDir();
    FString FileExt = TEXT("sav");

    IFileManager::Get().FindFiles(SaveFileNames, *Dir, *FileExt);
}

void UMainMenuWidget::OnGameLoadedFixup(UWorld* World)
{
}
