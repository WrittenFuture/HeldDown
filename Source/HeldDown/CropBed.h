// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeldDownCharacter.h"
#include "Inventory.h"
#include "InventoryItemStruct.h"
#include "CropBed.generated.h"

UCLASS()
class HELDDOWN_API ACropBed : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACropBed();

	UFUNCTION(BlueprintCallable)
	virtual void ActionButtonPressedOnThis();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AHeldDownCharacter* PlayerCharacter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	AInventory* PlayerInventory;

	TArray<FInventoryItemStruct> InventoryDatabase;

	void SetupPlayerInventory();

	TArray<FInventoryItemStruct> GetPlantableItemsInInventory();

	AHeldDownCharacter* PlayerCharecter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItemStruct> PlantableItemsInInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int> PositionsOfPlantableItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	int InventoryIndexOfPlantableItem;

	UFUNCTION(BlueprintCallable)
	int SelectPlantableItem();

	UFUNCTION(BlueprintCallable)
	void PlantCrop(int ItemInList);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
