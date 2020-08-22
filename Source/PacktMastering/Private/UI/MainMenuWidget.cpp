// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"

#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::LoadGame(FString SaveFile)
{
}

void UMainMenuWidget::SaveGame()
{
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
}

void UMainMenuWidget::OnGameLoadedFixup(UWorld* World)
{
}
