#pragma once

#include "CoreMinimal.h"
#include "InventoryItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FInventoryItemStruct
{
    GENERATED_BODY()

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    // Default constructor (needed!)
    FInventoryItemStruct()
        : Name(TEXT("Default")) {}

    // Optional: Parametrized constructor if you want it
    FInventoryItemStruct(FString InName, int32 InQuantity)
        : Name(InName) {}
};
