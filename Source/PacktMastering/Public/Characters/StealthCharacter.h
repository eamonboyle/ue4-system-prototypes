// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PacktMasteringCharacter.h"
#include "StealthCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PACKTMASTERING_API AStealthCharacter : public APacktMasteringCharacter
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    float StealthPitchYawScale = 0.5f;

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    virtual bool IsStealthed()
    {
        return bIsStealthed;
    }

protected:
    virtual void AddControllerPitchInput(float Val) override;
    virtual void AddControllerYawInput(float Val) override;

    void Stealth();
    void UnStealth();

protected:
    bool bIsStealthed = false;
};
