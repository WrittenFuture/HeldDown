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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    // Default constructor (needed!)
    FInventoryItemStruct()
        : Name(TEXT("Default")), Icon() {}

    // Optional: Parametrized constructor if you want it
    FInventoryItemStruct(FString InName, UTexture2D* InIcon)
        : Name(InName), Icon(InIcon) {}
};
