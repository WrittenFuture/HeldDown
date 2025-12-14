// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory.h"
#include "CollectableCrop.h"

// Sets default values
ACollectableCrop::ACollectableCrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACollectableCrop::BeginPlay()
{
	Super::BeginPlay();

	DirectChildComponents = GetChildComponents(empty);
	GreatGrandChildren = GetChildComponents(DirectChildComponents);

	MaxCropAmount = GreatGrandChildren.Num(); // Set maxCropAmount to the number of great-grandchildren
	AmountOnCrop = MaxCropAmount; // Initialize AmountOnCrop to maxCropAmount
	
}

// Called every frame
void ACollectableCrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CropReplenishTimer -= DeltaTime;
	if (CropReplenishTimer <= 0)
	{
		ReplenishCrop();
		//CropReplenishTimer = 100; // Reset the timer to 5 seconds
		CropReplenishTimer = 54000; // Reset the timer
	}

}

void ACollectableCrop::ActionButtonPressedOnThis()
{
	UE_LOG(LogTemp, Log, TEXT("Action button pressed on CollectableCrop"));
	CollectCrop();
}

void ACollectableCrop::CollectCrop()
{
	UE_LOG(LogTemp, Log, TEXT("Collecting crop..."));

	PlayerCharacter = Cast<AHeldDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()); //get inventory
	PlayerInventory = PlayerCharacter->GetPlayerInventory(); 

	if (PlayerInventory)
	{
		int InventorySpace = PlayerInventory->GetInventorySpace();

		if (InventorySpace <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("No inventory space left! Cannot collect crop."));
			return;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Inventory space available: %d"), InventorySpace);
		}
	}
	else 
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory is null! Cannot get inventory space."));
		return;
	}

	//fail safe
	if (!PlayerInventory)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory is null!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerInventory is valid!"));
	}
	

	int CropToCollect = FMath::Abs(AmountOnCrop - MaxCropAmount); // Calculate what crop to collect

	if (AmountOnCrop <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No crop left to collect!"));
		return;
	}
	GreatGrandChildren[CropToCollect]->SetVisibility(false, true); // false = invisible, true = apply to children
    GreatGrandChildren[CropToCollect]->SetHiddenInGame(true, true); // hides in game and optionally in editor

	UE_LOG(LogTemp, Log, TEXT("Collected %s "), *GreatGrandChildren[CropToCollect]->GetName());

	AmountOnCrop--;

	PlayerInventory->AddListToInventoryDatabase(ItemsCollected);

	PlayerInventory->PrintInventory();
	
}

void ACollectableCrop::ReplenishCrop()
{
	UE_LOG(LogTemp, Log, TEXT("Replenishing crop..."));

	// unhide all previously hidden components

	// play animation or sound here if needed

	for (USceneComponent* Child : GreatGrandChildren)
	{
		if (!Child->IsVisible())
		{
			Child->SetVisibility(true, true); // true = visible, true = apply to children
			Child->SetHiddenInGame(false, true); // unhides in game and optionally in editor
			UE_LOG(LogTemp, Log, TEXT("Replenished %s "), *Child->GetName());
			AmountOnCrop++;
		} 
		else 
		{
			UE_LOG(LogTemp, Warning, TEXT("Component %s is already visible!"), *Child->GetName());
		}
	}

	

}

TArray<USceneComponent*> ACollectableCrop::GetChildComponents(TArray<USceneComponent*> ParentComponents)
{

	TArray<USceneComponent*> ResultComponents;

	if (ParentComponents.IsEmpty())
	{
		GetRootComponent()->GetChildrenComponents(false, ResultComponents);

		for (USceneComponent* Child : ResultComponents)
		{
			UE_LOG(LogTemp, Log, TEXT("Child Component: %s"), *Child->GetName());
		}
		return ResultComponents;
	}
	else
	{
		for (USceneComponent* Child : ParentComponents)
		{

			Child->GetChildrenComponents(false, ResultComponents);
			for (USceneComponent* GrandChild : ResultComponents)
			{
				UE_LOG(LogTemp, Log, TEXT("GrandChild Component: %s"), *GrandChild->GetName());
			}
		}
		return ResultComponents;
	}
	
}

