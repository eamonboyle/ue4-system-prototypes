// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PacktMasteringHUD.generated.h"

UCLASS()
class APacktMasteringHUD : public AHUD
{
	GENERATED_BODY()

public:
	APacktMasteringHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

