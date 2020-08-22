// Fill out your copyright notice in the Description page of Project Settings.


#include "Cameras/IconCamera.h"


#include "HighResScreenshot.h"
#include "Camera/CameraComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/StreamableManager.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/WeaponPickup.h"

void AIconCamera::BeginPlay()
{
	if (bInitialized)
	{
		return; // BeginPlay will get called multiple times at level start
	}

	bInitialized = true;

	CameraComp = GetCameraComponent();

	UWorld* World = GetWorld();
	check(World != nullptr);
	APlayerController* Player = World->GetFirstPlayerController();

	Player->SetCinematicMode(true, true, true, true, true);

	Player->SetViewTarget(this);

	FString contentPath = FString("/Game/") + WeaponsPath;

	static UObjectLibrary* ObjectLibrary = nullptr;
	ObjectLibrary = UObjectLibrary::CreateLibrary(AWeaponPickup::StaticClass(), false, GIsEditor);
	ObjectLibrary->AddToRoot();
	ObjectLibrary->bHasBlueprintClasses = true;

	ObjectLibrary->LoadBlueprintAssetDataFromPath(contentPath);

	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);

	for (auto itr : AssetDatas)
	{
		FSoftObjectPath assetPath(itr.ObjectPath.ToString());
		WeaponBlueprintSoftRefs.Add(assetPath);
	}

	// Here we stream in the assets found that are weapon pick-ups and when done, will call the OnFinished function
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Streamable.RequestAsyncLoad(WeaponBlueprintSoftRefs, FStreamableDelegate::CreateUObject(this, &AIconCamera::OnFinishedLoadingAssets));
}

void AIconCamera::TakeShot()
{
	UWorld* World = GetWorld();

	check(CurrentWeaponPickup != nullptr);

	UMeshComponent* Mesh = Cast<UMeshComponent>(CurrentWeaponPickup->GetComponentByClass(UMeshComponent::StaticClass()));
	check(Mesh != nullptr);

	Mesh->bForceMipStreaming = true;

	Mesh->SetRenderCustomDepth(true);

	GScreenshotResolutionX = ScreenshotResolutionX;
	GScreenshotResolutionY = ScreenshotResolutionY;

	GetHighResScreenshotConfig().SetHDRCapture(true);
	GetHighResScreenshotConfig().bMaskEnabled = true;
	World->GetGameViewport()->Viewport->TakeHighResScreenShot();

	// this timer is here to wait just one frame (hence the tiny time) and then destroy the current actor
	// and spawn the next one: if you destroy the actor the same frame as the screenshot it may not appear
	FTimerHandle SpawnNextTimer;
	World->GetTimerManager().SetTimer(SpawnNextTimer, [this] {
		if (CurrentWeaponIndex >= WeaponBlueprints.Num())
		{
			CurrentWeaponIndex = 0; // we have finished, this will break our timer loop on its next trigger
		}
		else
		{
			SpawnAndPlaceNextActor();
		}
	},
	0.001f, false);
}

void AIconCamera::SpawnAndPlaceNextActor()
{
	if (CurrentWeaponPickup != nullptr)
		CurrentWeaponPickup->Destroy();

	CurrentWeaponBlueprint = WeaponBlueprints[CurrentWeaponIndex];
	check(CurrentWeaponBlueprint != nullptr); // anything not a blueprint should never find its way into our list

	UWorld* World = GetWorld();

	FRotator Rot(0.0f);
	FVector Trans(0.0f);

	FTransform Transform(Rot, Trans);
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeaponPickup = World->SpawnActor<AWeaponPickup>(CurrentWeaponBlueprint->GeneratedClass, Transform, ActorSpawnParams);
	CurrentWeaponPickup->RotationSpeed = 0.0f; // the ones we use for screenshots we don't want spinning!
	check(CurrentWeaponPickup != nullptr);

	FVector Pos = ComputeActorLocation();
	CurrentWeaponPickup->SetActorLocation(Pos);

	CurrentWeaponIndex++;
}

FVector AIconCamera::ComputeActorLocation()
{
	check(CurrentWeaponPickup != nullptr);
	UMeshComponent* Mesh = Cast<UMeshComponent>(CurrentWeaponPickup->GetComponentByClass(UMeshComponent::StaticClass()));

	FVector InPos;
	FVector BoxExtent;
	CurrentWeaponPickup->GetActorBounds(false, InPos, BoxExtent);

	// uncomment these to view the actor bounding generated for our pick-ups
	/*FVector CurrentPosition = CurrentWeaponPickup->GetActorLocation();
	FColor fcRandom(FMath::RandRange(64, 255), FMath::RandRange(64, 255), FMath::RandRange(64, 255));
	DrawDebugLine(World, CurrentPosition, CurrentPosition + InPos, fcRandom, false, 20.0f);
	DrawDebugBox(World, CurrentPosition + InPos, 0.5f * BoxExtent, FQuat(ForceInitToZero), fcRandom, false, 20.0f);*/

	// uncomment these to view the mesh bounding imported with the assets
	/*FBoxSphereBounds bsMesh = Mesh->Bounds;
	DrawDebugLine(World, CurrentPosition, bsMesh.Origin, fcRandom, false, 20.0f);
	DrawDebugBox(World, bsMesh.Origin, 0.5f * bsMesh.BoxExtent, FQuat(ForceInitToZero), fcRandom, false, 20.0f);*/

	const float fX = BoxExtent.X;
	const float fY = BoxExtent.Y;
	const float fZ = BoxExtent.Z;

	if (fX > fY)
	{
		FRotator YawRot(0.0f, 90.0f, 0.0f);
		CurrentWeaponPickup->SetActorRotation(YawRot);
	}

	const float fLongestBoxSide = FMath::Max(fX, FMath::Max(fY, fZ));

	// FOV is the whole frustum FOV, to make a right triangle down its middle, we use half this angle
	const float FOVhalf = 0.5f * CameraComp->FieldOfView;
	const float FOVradians = FOVhalf * PI / 180.0f;

	const float FOVtan = FMath::Tan(FOVradians);

	float XDistance = fLongestBoxSide / FOVtan;

	FVector Positioning(XDistance, 0.0f, 0.0f);

	return CurrentWeaponPickup->GetActorLocation() + Positioning - InPos;
}

void AIconCamera::OnFinishedLoadingAssets()
{
	UWorld* World = GetWorld();

	for (auto itr = WeaponBlueprintSoftRefs.CreateIterator(); itr; ++itr)
	{
		UBlueprint *BPObj = CastChecked<UBlueprint>((*itr).ResolveObject());
		WeaponBlueprints.Add(BPObj);
	}

	SpawnAndPlaceNextActor(); // this spawns our first pickup and increments CurrentWeaponIndex to 1

	static FTimerHandle ScreenShotTimer;
	World->GetTimerManager().SetTimer(ScreenShotTimer, [=] {
			if (CurrentWeaponIndex == 0) // only way we come in at index 0 is if we're done
			{
				World->GetTimerManager().ClearTimer(ScreenShotTimer);
				if (APlayerController* Player = UGameplayStatics::GetPlayerController(World, 0))
				{
					Player->ConsoleCommand(TEXT("Exit"), true);
					return;
				}
			}

			TakeShot();
		},
		ShotDelay, true, ShotDelay);
}