// Fill out your copyright notice in the Description page of Project Settings.


#include "CropBed.h"
#include "HeldDownCharacter.h"
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
	for (FInventoryItemStruct& Items : InventoryDatabase)
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
	UStaticMeshComponent* MeshRoot = Cast<UStaticMeshComponent>(GetRootComponent());
	if (!PlantableItemsInInventory.IsValidIndex(ItemInList))
	{
		UE_LOG(LogTemp, Error, TEXT("ItemInList is not valid!"));
		return;
	}
	UStaticMesh* Mesh = PlantableItemsInInventory[ItemInList].MeshesForCropBed[0];
	FInventoryItemStruct ItemPlanted = PlantableItemsInInventory[ItemInList];
	USceneComponent* Root = GetRootComponent();
	UE_LOG(LogTemp, Log, TEXT("Planting crop..."));
	if (Mesh){
		UStaticMeshComponent* NewCropMesh = NewObject<UStaticMeshComponent>(this);
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

}

int ACropBed::SelectPlantableItem()
{
	//player will select item from UI, for now we will just return the first item
	UE_LOG(LogTemp, Log, TEXT("Selecting plantable item..."));

	//this will set the index in the inventorydatabase of the item that is selected
	InventoryIndexOfPlantableItem = 0; // This should be replaced with actual selection logic

	return 0; // This should be replaced with actual selection logic
}

void ACropBed::ActionButtonPressedOnThis()
{
	UE_LOG(LogTemp, Log, TEXT("Action button pressed on CollectableCrop"));
	SetupPlayerInventory();
	GetPlantableItemsInInventory();
	PlantCrop(SelectPlantableItem());
	PlantableItemsInInventory.Empty(); // Clear the list after planting

}

