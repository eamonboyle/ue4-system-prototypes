// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MeleeAIController.generated.h"

/**
 * 
 */
UCLASS()
class PACKTMASTERING_API AMeleeAIController : public AAIController
{
    GENERATED_BODY()

public:
    AMeleeAIController();

    UFUNCTION(BlueprintImplementableEvent)
    void OnTargetChange(class APacktMasteringCharacter* Target);

    virtual void OnPossess(APawn* InPawn) override;

    UFUNCTION(BlueprintCallable)
    class APacktMasteringCharacter* GetTarget();

    UFUNCTION(BlueprintCallable)
    void SetReturningHome();

    UFUNCTION(BlueprintCallable)
    void OnReturnedHome();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Targeting)
    class USphereComponent* HearingSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Targeting)
    class USphereComponent* StealthHearingSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Targeting)
    class USphereComponent* SightSphere;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Targeting)
    float HearingRadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Targeting)
    float StealthHearingRadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Targeting)
    float SightRadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Targeting)
    float SightAngle;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Targeting)
    float AttackRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame, Category = Health)
    float Health;

protected:
    UFUNCTION()
    void OnHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnStealthHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                                 int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSightOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp,
                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void SetPotentialTarget(AActor* Other);

    UPROPERTY()
    APacktMasteringCharacter* CurrentTarget;

    UPROPERTY(BlueprintReadOnly)
    FVector HomeLocation;
};
