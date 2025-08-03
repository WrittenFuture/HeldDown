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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> Attributes;

    // for crops and plantable items
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UStaticMesh*> MeshesForCropBed; // Optional: If you want to include meshes

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int GrowthTime; // Optional: If you want to include growth time in seconds

    
    

    // Default constructor (needed!)
    FInventoryItemStruct()
        : Name(TEXT("Default")), Icon(), Attributes(), MeshesForCropBed(), GrowthTime() {}

    // Optional: Parametrized constructor if you want it
    FInventoryItemStruct(FString InName, UTexture2D* InIcon, TArray<FString> InAttributes, TArray<UStaticMesh*> InMeshesForCropBed, int InGrowthTime)
        : Name(InName), Icon(InIcon), Attributes(InAttributes), MeshesForCropBed(InMeshesForCropBed), GrowthTime(InGrowthTime) {}
};
