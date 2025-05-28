// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Inventory.h"
#include "HeldDownCharacter.h"
#include "CollectableCrop.generated.h"

UCLASS()
class HELDDOWN_API ACollectableCrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableCrop();

protected:

	int CropAmount = 3; // Amount of crop that can be collected
	int MaxCropAmount = 10; // Maximum amount of crop that can be collected
	int AmountOnCrop = 0;
	int CropReplenishTimer = 5; // Time in seconds to replenish the crop

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TArray<USceneComponent*> CollectableComponents;

	TArray<USceneComponent*> empty;
	TArray<USceneComponent*> DirectChildComponents;
	TArray<USceneComponent*> GreatGrandChildren;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
	AHeldDownCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	AInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryItemStruct> ItemsCollected;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void CollectCrop();

	virtual void ReplenishCrop();

	virtual TArray<USceneComponent*> GetChildComponents(TArray<USceneComponent*>);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void ActionButtonPressedOnThis();

};
