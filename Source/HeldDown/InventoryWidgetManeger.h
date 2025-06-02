// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory.h"
#include "HeldDownCharacter.h"
#include "Engine/Texture2D.h"
#include "InventoryWidgetManeger.generated.h"

/**
 * 
 */
UCLASS()
class HELDDOWN_API UInventoryWidgetManeger : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetPlayerInventory();

	virtual void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory")
	void UpdateInventoryUI();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	AInventory* PlayerInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	AHeldDownCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<UTexture2D*> InventorySlotTexture;

	
};
