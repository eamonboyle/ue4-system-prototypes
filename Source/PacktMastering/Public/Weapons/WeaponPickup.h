// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"

UCLASS()
class PACKTMASTERING_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponPickup();

	void HavePlayerPickup(class APacktMasteringCharacter* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTexture2D* InventoryIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float RotationSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 Ammunition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 WeaponPower;
};
