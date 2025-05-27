// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeldDownCharacter.h"
#include "ToggleableLights.generated.h"


UCLASS()
class HELDDOWN_API AToggleableLights : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AToggleableLights();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void TurnOnLights();
	virtual void TurnOffLights();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "casting")
	AHeldDownCharacter* Player;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Lighting")
	bool IsLightsOn = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void ActionButtonPressedOnThis();

	UFUNCTION(BlueprintCallable)
	virtual void ActionButtonReleasedOnThis();

};
