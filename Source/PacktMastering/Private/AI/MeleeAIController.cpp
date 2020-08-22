// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MeleeAIController.h"


#include "Characters/StealthCharacter.h"
#include "Components/SphereComponent.h"

AMeleeAIController::AMeleeAIController()
{
    HearingRadius = 600.f;
    StealthHearingRadius = 150.f;
    SightRadius = 1800.f;
    SightAngle = 75.f;
    AttackRadius = 120.f;
    Health = 100.f;
    CurrentTarget = nullptr;
    
    HearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HearingSphere"));
    HearingSphere->SetCollisionObjectType(ECC_Pawn);
    HearingSphere->SetCollisionProfileName("Trigger");
    HearingSphere->SetSphereRadius(HearingRadius);
    HearingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnHearingOverlap);

    StealthHearingSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StealthHearingSphere"));
    StealthHearingSphere->SetCollisionObjectType(ECC_Pawn);
    StealthHearingSphere->SetCollisionProfileName("Trigger");
    StealthHearingSphere->SetSphereRadius(StealthHearingRadius);
    StealthHearingSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnStealthHearingOverlap);

    SightSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SightSphere"));
    SightSphere->SetCollisionObjectType(ECC_Pawn);
    SightSphere->SetCollisionProfileName("Trigger");
    SightSphere->SetSphereRadius(SightRadius);
    SightSphere->OnComponentBeginOverlap.AddDynamic(this, &AMeleeAIController::OnSightOverlap);

    SetReturningHome();

    bAttachToPawn = true;
}

void AMeleeAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    HomeLocation = GetPawn()->GetNavAgentLocation();

    HearingSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    StealthHearingSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    SightSphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    OnReturnedHome();
}

APacktMasteringCharacter* AMeleeAIController::GetTarget()
{
    return CurrentTarget;
}

void AMeleeAIController::SetReturningHome()
{
    HearingSphere->SetSphereRadius(0.f);
    StealthHearingSphere->SetSphereRadius(0.f);
    SightSphere->SetSphereRadius(0.f);

    CurrentTarget = nullptr;

    UE_LOG(LogTemp, Warning, TEXT("SET RETURNING HOME"));
}

void AMeleeAIController::OnReturnedHome()
{
    HearingSphere->SetSphereRadius(HearingRadius);
    StealthHearingSphere->SetSphereRadius(StealthHearingRadius);
    SightSphere->SetSphereRadius(SightRadius);

    UE_LOG(LogTemp, Warning, TEXT("Returned HOME"));
}

void AMeleeAIController::OnHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AStealthCharacter* StealthCharacter = Cast<AStealthCharacter>(Other);
    if (StealthCharacter != nullptr)
    {
        if (StealthCharacter->IsStealthed())
        {
            return;
        }
    }

    SetPotentialTarget(Other);
}

void AMeleeAIController::OnStealthHearingOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    SetPotentialTarget(Other);
}

void AMeleeAIController::OnSightOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APawn* MyOwner = GetPawn();

    if (MyOwner != nullptr || MyOwner == Other)
    {
        return;
    }

    FVector ToTarget = Other->GetActorLocation() - MyOwner->GetActorLocation();
    FVector Facing = GetPawn()->GetActorForwardVector();

    if (SightAngle > 90.f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sight angles of 90+ degrees not supported, please use hearing for this detection"));
        SightAngle = 90.f;
    }

    if (FVector::DotProduct(ToTarget, Facing) < 0.f)
    {
        return;
    }

    float DotToTarget = FVector::DotProduct(ToTarget.GetSafeNormal(), Facing.GetSafeNormal());
    float RadiansToTarget = FMath::Acos(DotToTarget);
    float AngTotarget = RadiansToTarget * 180.f / PI;

    if (AngTotarget < SightAngle)
    {
        SetPotentialTarget(Other);
    }
}

void AMeleeAIController::SetPotentialTarget(AActor* Other)
{
    APacktMasteringCharacter* Target = Cast<APacktMasteringCharacter>(Other);
    if (Target != nullptr && CurrentTarget != Target)
    {
        CurrentTarget = Target;
        OnTargetChange(CurrentTarget);
    }
}
