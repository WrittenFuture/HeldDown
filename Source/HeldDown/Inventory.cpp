// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory.h"
#include "InventoryItemStruct.h"

// Sets default values
AInventory::AInventory()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInventory::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("AInventory::BeginPlay called"));
	PrintInventory(); // Print the inventory to the log for debugging
	InventoryDatabase.Empty(); // Initialize the inventory database
	UE_LOG(LogTemp, Log, TEXT("AInventory::BeginPlay after InventoryDatabase.Empty() called"));
	PrintInventory(); // Print the inventory to the log for debugging
	
	InventorySize = 6; // Set default inventory size
}

// Called every frame
void AInventory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInventory::SetInventoryDatabase(TArray<FInventoryItemStruct> NewInventoryData)
{
	InventoryDatabase = NewInventoryData;
}

TArray<FInventoryItemStruct> AInventory::GetInventoryDatabase()
{
	return InventoryDatabase;
}

void AInventory::ReplaceInInventoryDatabase(int index, FInventoryItemStruct NewItem)
{
	UE_LOG(LogTemp, Log, TEXT("ReplaceInInventoryDatabase called"));
}

void AInventory::AddToInventoryDatabase(FInventoryItemStruct NewItem)
{
	UE_LOG(LogTemp, Log, TEXT("AddToInventoryDatabase called"));
	InventoryDatabase.Add(NewItem);
}

void AInventory::AddListToInventoryDatabase(TArray<FInventoryItemStruct> ListOfNewItem)
{
	UE_LOG(LogTemp, Log, TEXT("AddListToInventoryDatabase called"));
	InventoryDatabase.Append(ListOfNewItem);
	ListOfNewItem.Empty(); // Clear the list after adding to the inventory
}

TArray<FInventoryItemStruct> AInventory::SwapItemInInventoryDatabase(int index1, int index2)
{
	UE_LOG(LogTemp, Log, TEXT("SwapItemInInventoryDatabase called"));
	return InventoryDatabase; // This is a placeholder, implement the actual swap logic
}

void AInventory::PrintInventory()
{
	int itteration = 0;
	for (FInventoryItemStruct Item : InventoryDatabase)
	{
		UE_LOG(LogTemp, Log, TEXT("Item %d:"), itteration++);
		UE_LOG(LogTemp, Log, TEXT("Item Name: %s"), *Item.Name);
	}
}

void AInventory::RemoveFromInventoryDatabaseByInt(int index)
{
	if (InventoryDatabase.IsValidIndex(index))
	{
		InventoryDatabase.RemoveAt(index);
		UE_LOG(LogTemp, Log, TEXT("Removed item at index %d"), index);

		for (int i = index; i < InventoryDatabase.Num(); i++)
		{
			if (!InventoryDatabase.IsValidIndex(i + 1))
			{
				break; // Prevent out-of-bounds access
			}
			InventoryDatabase[i] = InventoryDatabase[i + 1]; // Update the index of the remaining items
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid index %d for removal"), index);
	}
}


int AInventory::GetInventorySpace()
{
    int UsedSlots = InventoryDatabase.Num();
    int SpaceLeft = InventorySize - UsedSlots;

    UE_LOG(LogTemp, Log, TEXT("Current inventory space: %d"), SpaceLeft);
    return SpaceLeft;
}

