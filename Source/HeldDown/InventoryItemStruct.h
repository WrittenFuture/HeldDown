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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UStaticMesh*> MeshesForCropBed; // Optional: If you want to include meshes

    // Default constructor (needed!)
    FInventoryItemStruct()
        : Name(TEXT("Default")), Icon(), Attributes(), MeshesForCropBed() {}

    // Optional: Parametrized constructor if you want it
    FInventoryItemStruct(FString InName, UTexture2D* InIcon, TArray<FString> InAttributes, TArray<UStaticMesh*> InMeshesForCropBed)
        : Name(InName), Icon(InIcon), Attributes(InAttributes), MeshesForCropBed(InMeshesForCropBed) {}
};
