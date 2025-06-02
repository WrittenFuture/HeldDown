// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidgetManeger.h"

// manually called by AHeldDownCharacter::BeginPlay() afetr inventory is spawned. this is why i can get the inventory here
void UInventoryWidgetManeger::BeginPlay()
{
    PlayerCharacter = Cast<AHeldDownCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
}


void UInventoryWidgetManeger::SetPlayerInventory()
{
    if (PlayerCharacter)
    {
        PlayerInventory = PlayerCharacter->GetPlayerInventory();
        if (!PlayerInventory)
        {
            UE_LOG(LogTemp, Error, TEXT("PlayerInventory is null!"));
            return;
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("PlayerInventory is valid!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is null!"));
        return;
    }

    for (int i = 0; i <= 5; i++)
    {
        if (PlayerInventory->GetInventoryDatabase().IsValidIndex(i))
        {
            UE_LOG(LogTemp, Log, TEXT("Adding item %d to InventorySlotTexture"), i);
            InventorySlotTexture.Add(PlayerInventory->GetInventoryDatabase()[i].Icon);
        }
    }
}
