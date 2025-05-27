// Fill out your copyright notice in the Description page of Project Settings.


#include "ToggleableLights.h"
#include "HeldDownCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AToggleableLights::AToggleableLights()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AToggleableLights::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AHeldDownCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
}

void AToggleableLights::TurnOnLights()
{
	UE_LOG(LogTemp, Log, TEXT("Lights are ON"));
}

void AToggleableLights::TurnOffLights()
{
	UE_LOG(LogTemp, Log, TEXT("Lights are OFF"));
}

void AToggleableLights::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AToggleableLights::ActionButtonPressedOnThis()
{
	UE_LOG(LogTemp, Log, TEXT("Action button pressed on this object"));
	if (!IsLightsOn)
	{
		TurnOnLights();
		IsLightsOn = true;
	}
	else
	{
		TurnOffLights();
		IsLightsOn = false;
	}
}

void AToggleableLights::ActionButtonReleasedOnThis()
{
	UE_LOG(LogTemp, Log, TEXT("Action button released on this object"));
	TurnOffLights();
}

