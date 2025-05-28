// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryItemStruct.h"
#include "Inventory.generated.h"

UCLASS()
class HELDDOWN_API AInventory : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AInventory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItemStruct> InventoryDatabase;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItemStruct> GetInventoryDatabase();
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryDatabase(TArray<FInventoryItemStruct> NewInventoryData);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void ReplaceInInventoryDatabase(int index, FInventoryItemStruct NewItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddToInventoryDatabase(FInventoryItemStruct NewItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddListToInventoryDatabase(TArray<FInventoryItemStruct> ListOfNewItem);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FInventoryItemStruct> SwapItemInInventoryDatabase(int index1, int index2);

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void PrintInventory();

};
