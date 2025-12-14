// Fill out your copyright notice in the Description page of Project Settings.

#include "CropBed.h"
#include "HeldDownCharacter.h"
#include "TimerManager.h"
#include "Inventory.h"

// Sets default values
ACropBed::ACropBed()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACropBed::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("ACropBed BeginPlay called"));
}

// Called every frame
void ACropBed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACropBed::SetupPlayerInventory()
{
	PlayerCharacter = Cast<AHeldDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (PlayerCharacter)
	{
		PlayerInventory = PlayerCharacter->GetPlayerInventory();
		if (!PlayerInventory)
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerInventory is null!"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("PlayerInventory is valid!"));
			InventoryDatabase = PlayerInventory->GetInventoryDatabase();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null!"));
	}
}

TArray<FInventoryItemStruct> ACropBed::GetPlantableItemsInInventory()
{
	UE_LOG(LogTemp, Log, TEXT("Getting plantable items in inventory..."));
	int index = 0;
	for (FInventoryItemStruct &Items : InventoryDatabase)
	{
		UE_LOG(LogTemp, Log, TEXT("Item: %s"), *Items.Name);
		if (Items.Attributes.Contains("Growable"))
		{
			PlantableItemsInInventory.Add(Items);
			UE_LOG(LogTemp, Log, TEXT("Found plantable item: %s"), *Items.Name);
			PositionsOfPlantableItems.Add(index);
			UE_LOG(LogTemp, Log, TEXT("Position of plantable item: %d"), index);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Item %s is not plantable."), *Items.Name);
		}
		index++;
	}

	return PlantableItemsInInventory;
}

void ACropBed::PlantCrop(int ItemInList)
{
	UStaticMeshComponent *MeshRoot = Cast<UStaticMeshComponent>(GetRootComponent());
	if (!PlantableItemsInInventory.IsValidIndex(ItemInList))
	{
		UE_LOG(LogTemp, Error, TEXT("ItemInList is not valid!"));
		return;
	}
	UStaticMesh *Mesh = PlantableItemsInInventory[ItemInList].MeshesForCropBed[0];
	FInventoryItemStruct ItemPlanted = PlantableItemsInInventory[ItemInList];
	USceneComponent *Root = GetRootComponent();
	UE_LOG(LogTemp, Log, TEXT("Planting crop..."));
	if (Mesh)
	{
		UStaticMeshComponent *NewCropMesh = NewObject<UStaticMeshComponent>(this);
		if (Root)
		{
			NewCropMesh->SetStaticMesh(Mesh);
			NewCropMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			NewCropMesh->RegisterComponent();

			if (ItemPlanted.Name.IsEmpty())
			{
				PlayerInventory->RemoveFromInventoryDatabaseByInt(ItemInList);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MeshRoot is null!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is null!"));
		return;
	}

	if (PlayerInventory)
	{
		PlayerInventory->RemoveFromInventoryDatabaseByInt(InventoryIndexOfPlantableItem);
		UE_LOG(LogTemp, Log, TEXT("Removed item from inventory database at position %d"), PositionsOfPlantableItems[ItemInList]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerInventory is null! Cannot remove item from inventory."));
	}

	// assogn vars for the growth of the crop

	if (ItemPlanted.GrowthTime > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Crop will grow for in increments of %d."), ItemPlanted.GrowthTime);
		CropGrowthTime = ItemPlanted.GrowthTime;
	}

	if (ItemPlanted.MeshesForCropBed.Num() > 1)
	{
		UE_LOG(LogTemp, Log, TEXT("Crop has multiple meshes for growth stages."));
		MeshsForGrowthStages = ItemPlanted.MeshesForCropBed;
	}

	CropGrowthStage = 0; // Start at the first growth stage

	// Start a timer to progress the crop growth
	// Start repeating timer
	GetWorldTimerManager().SetTimer(
		CropGrowthTimer,		 // Timer handle
		this,					 // Object to call the function on
		&ACropBed::ProgressCrop, // Function to call
		CropGrowthTime,			 // Time interval
		true					 // Looping = true
	);
}


// TODO: Both ProressCrop and HarvestCrop delete previous mesh based on name, this stops working after the first harvest
// either the name needs to be reset to StaticMeshComponent_0 wich i believe is managed by the engine 
//or we need to find a better way to handle this


void ACropBed::ProgressCrop()
{
	CropGrowthStage++;
	UE_LOG(LogTemp, Log, TEXT("Progressing crop growth to stage %d."), CropGrowthStage);

	UStaticMeshComponent *MeshRoot = Cast<UStaticMeshComponent>(GetRootComponent());
	UStaticMesh *Mesh = MeshsForGrowthStages[0];
	USceneComponent *Root = GetRootComponent();
	FString PrevMeshName = FString::Printf(TEXT("StaticMeshComponent_%d"), CropGrowthStage - 1);

	if (CropGrowthStage >= MeshsForGrowthStages.Num())
	{

		//allow crop to be harvested when this happens
		ReadyToHarvest = true;

		UE_LOG(LogTemp, Log, TEXT("Crop has reached the maximum growth stage. No more growth stages available."));
		GetWorldTimerManager().ClearTimer(CropGrowthTimer); // Stop the timer
		return;												// Stop if we exceed the number of growth stages
	}
	else
	{
		Mesh = MeshsForGrowthStages[CropGrowthStage]; // Get the mesh for the current growth stage here instead
	}
	if (Mesh)
	{
		UStaticMeshComponent *NewCropMesh = NewObject<UStaticMeshComponent>(this);
		if (Root)
		{

			TArray<UStaticMeshComponent *> MeshComponents;
			GetComponents<UStaticMeshComponent>(MeshComponents);

			for (UStaticMeshComponent *MeshComp : MeshComponents)
			{
				if (MeshComp)
				{
					UE_LOG(LogTemp, Log, TEXT("MeshComp: %s"), *MeshComp->GetName());
				}
				if (MeshComp && MeshComp->GetName() == PrevMeshName)
				{
					MeshComp->DestroyComponent();
        			UE_LOG(LogTemp, Log, TEXT("Destroyed mesh component: %s"), *PrevMeshName);
        			break; 
				}
			}

			NewCropMesh->SetStaticMesh(Mesh);
			NewCropMesh->AttachToComponent(Root, FAttachmentTransformRules::KeepRelativeTransform);
			NewCropMesh->RegisterComponent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MeshRoot is null!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is null!"));
		return;
	}
}

int ACropBed::SelectPlantableItem()
{
	// player will select item from UI, for now we will just return the first item
	UE_LOG(LogTemp, Log, TEXT("Selecting plantable item..."));

	// this will set the index in the inventorydatabase of the item that is selected
	InventoryIndexOfPlantableItem = 0; // This should be replaced with actual selection logic

	return 0; // This should be replaced with actual selection logic
}

void ACropBed::HarvestCrop()
{
	UE_LOG(LogTemp, Log, TEXT("Harvesting crop..."));

	UStaticMeshComponent *MeshRoot = Cast<UStaticMeshComponent>(GetRootComponent());
	UStaticMesh *Mesh = MeshsForGrowthStages[0];
	USceneComponent *Root = GetRootComponent();
	FString PrevMeshName = FString::Printf(TEXT("StaticMeshComponent_%d"), CropGrowthStage - 1);

	if (Mesh)
	{
		UStaticMeshComponent *NewCropMesh = NewObject<UStaticMeshComponent>(this);
		if (Root)
		{

			TArray<UStaticMeshComponent *> MeshComponents;
			GetComponents<UStaticMeshComponent>(MeshComponents);

			for (UStaticMeshComponent *MeshComp : MeshComponents)
			{
				if (MeshComp && MeshComp->GetName() == PrevMeshName)
				{
					MeshComp->DestroyComponent();
        			UE_LOG(LogTemp, Log, TEXT("Destroyed mesh component: %s"), *PrevMeshName);
        			break; // Remove this line if you expect multiple with the same name
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MeshRoot is null!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh is null!"));
		return;
	}

	CropGrowthStage = 0; // Reset the growth stage after harvesting
	ReadyToHarvest = false; // Reset the harvest state

	// TODO: Add logic to give the player the harvested item


}

void ACropBed::ActionButtonPressedOnThis()
{
	UE_LOG(LogTemp, Log, TEXT("Action button pressed on CollectableCrop"));
	SetupPlayerInventory();
	GetPlantableItemsInInventory();
	PlantCrop(SelectPlantableItem());
	PlantableItemsInInventory.Empty(); // Clear the list after planting

	// if crop is ready to be harvested, we can call a function to collect it
	if(ReadyToHarvest)
	{
		HarvestCrop();
	}
}
