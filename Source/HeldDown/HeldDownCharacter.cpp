// Copyright Epic Games, Inc. All Rights Reserved.

#include "HeldDownCharacter.h"
#include "HeldDownProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Inventory.h"
#include "InventoryWidgetManeger.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AHeldDownCharacter

AHeldDownCharacter::AHeldDownCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

//////////////////////////////////////////////////////////////////////////// Input

void AHeldDownCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Initialize the inventory
	PlayerInventory = GetWorld()->SpawnActor<AInventory>(AInventory::StaticClass());
	PlayerController = Cast<APlayerController>(GetController());
}

void AHeldDownCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerControllerForMapping = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerForMapping->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AHeldDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHeldDownCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHeldDownCharacter::Look);

		// Action button pressed
		EnhancedInputComponent->BindAction(ActionButton, ETriggerEvent::Started, this, &AHeldDownCharacter::ActionButtonPressed);
		EnhancedInputComponent->BindAction(ActionButton, ETriggerEvent::Completed, this, &AHeldDownCharacter::ActionButtonReleased);

		EnhancedInputComponent->BindAction(OpenMainMenuAction, ETriggerEvent::Started, this, &AHeldDownCharacter::OpenMainMenu);

		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AHeldDownCharacter::OpenInventory);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AHeldDownCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AHeldDownCharacter::Look(const FInputActionValue& Value)
{
	if (CanLook)
	{
		// input is a Vector2D
		FVector2D LookAxisVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add yaw and pitch input to controller
			AddControllerYawInput(LookAxisVector.X);
			AddControllerPitchInput(LookAxisVector.Y);
		}
	}
}

void AHeldDownCharacter::ActionButtonPressed(const FInputActionValue& Value)
{
	// input is a Vector2D
	IsActionButtonPressed = true;
	float ViewDistance = 1000.0f;
	AactorViewed = AHeldDownCharacter::ItemViewed(ViewDistance);

	

	if (AactorViewed)
	{
		UFunction* RecivingFunc = AactorViewed->FindFunction(FName("ActionButtonPressedOnThis"));
		UE_LOG(LogTemplateCharacter, Log, TEXT("ActionButtonPressed on %s"), *AactorViewed->GetName());

		if (RecivingFunc)
		{
			AactorViewed->ProcessEvent(AactorViewed->FindFunction(FName("ActionButtonPressedOnThis")), nullptr);
		}
		
	}
	
}

void AHeldDownCharacter::ActionButtonReleased(const FInputActionValue& Value)
{
	// input is a Vector2D
	IsActionButtonPressed = false;
	AactorViewed = nullptr;
}

void AHeldDownCharacter::GetActionButtonInfo(AActor*& ItemViewed, bool& IsPressed)
{
	ItemViewed = AactorViewed;
	IsPressed = IsActionButtonPressed;
}

AActor* AHeldDownCharacter::ItemViewed(float ViewDistance)
{
	FVector Start = FVector::ZeroVector;
    FRotator Rotation = FRotator::ZeroRotator;

    // Get the player's viewpoint
    GetController()->GetPlayerViewPoint(Start, Rotation);

    FVector End = Start + (Rotation.Vector() * ViewDistance);

    FHitResult HitItem;
    FCollisionQueryParams TraceParams(FName(TEXT("LookTrace")), true, this);
    TraceParams.bReturnPhysicalMaterial = false;
    TraceParams.AddIgnoredActor(this);

    // Perform line trace (raycast)
    if (GetWorld()->LineTraceSingleByChannel(HitItem, Start, End, ECC_Visibility, TraceParams))
    {
        return HitItem.GetActor();
    }

    return nullptr;
}

float AHeldDownCharacter::GetHealth()
{
	return Health;
}

float AHeldDownCharacter::GetHunger()
{
	return Hunger;
}

void AHeldDownCharacter::SetHealth(float NewHealth)
{
	if (NewHealth < 0)
	{
		NewHealth = 0;
		return;
	}
	else if (NewHealth > 100)
	{
		NewHealth = 100;
		return;
	}else{
		Health = NewHealth;
	}
	
}

void AHeldDownCharacter::SetHunger(float NewHunger)
{
	if (NewHunger < 0)
	{
		NewHunger = 0;
		return;
	}
	else if (NewHunger > 100)
	{
		NewHunger = 100;
		return;
	}
	else
	{
		Hunger = NewHunger;
	}
}

void AHeldDownCharacter::SetShouldDisplayStatUI(bool NewShouldDisplayStats)
{
	ShouldDisplayStatUI = NewShouldDisplayStats;
}

bool AHeldDownCharacter::GetShouldDisplayStatUI()
{
	return ShouldDisplayStatUI;
}

void AHeldDownCharacter::SetCanLook(bool NewCanLook)
{
	CanLook = NewCanLook;
}

bool AHeldDownCharacter::GetCanLook()
{
	return CanLook;
}

void AHeldDownCharacter::OpenMainMenu()
{
	UE_LOG(LogTemplateCharacter, Log, TEXT("OpenMainMenu called"));
	ShouldOpenMainMenue = !ShouldOpenMainMenue;
	if (ShouldOpenMainMenue)
	{
		UE_LOG(LogTemplateCharacter, Log, TEXT("Open Main Menu"));
		if (MainMenueParentClass)
		{
			UE_LOG(LogTemplateCharacter, Log, TEXT("MainMenueParentClass is valid"));
			MainMenueInstance = CreateWidget<UUserWidget>(GetWorld(), MainMenueParentClass);
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("MainMenueParentClass is not valid"));
			return;
		}

		

		if (MainMenueInstance)
		{
			UE_LOG(LogTemplateCharacter, Log, TEXT("MainMenueInstance is valid"));
			MainMenueInstance->AddToViewport();
			CanLook = false; // Disable looking when the main menu is open

			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
        		PlayerController->bEnableClickEvents = true;
        		PlayerController->bEnableMouseOverEvents = true;

			}
			else
			{
				UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to get PlayerController"));
			}
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to create MainMenueInstance"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Log, TEXT("Close Main Menu"));
		if (MainMenueInstance)
		{
			MainMenueInstance->RemoveFromParent();
			MainMenueInstance = nullptr;

			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = false;
				PlayerController->bEnableClickEvents = false;
				PlayerController->bEnableMouseOverEvents = false;
			}
			else
			{
				UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to get PlayerController"));
			}
			CanLook = true; // Re-enable looking when the main menu is closed
		}
		else
		{
			UE_LOG(LogTemplateCharacter, Warning, TEXT("MainMenueInstance is already null"));
		}
	}
	
}

AInventory* AHeldDownCharacter::GetPlayerInventory()
{
	return PlayerInventory;
}

void AHeldDownCharacter::OpenInventory()
{
	ShouldOpenInventory = !ShouldOpenInventory;

	if(ShouldOpenInventory)
	{
		UE_LOG(LogTemp, Log, TEXT("OpenInventory called"));
		if (InventoryWidgetManegerClass)
		{
			UE_LOG(LogTemp, Log, TEXT("InventoryWidgetManegerClass is valid"));

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("InventoryWidgetManegerClass is not valid"));
			return;
		}

		InventoryWidgetInstance = CreateWidget<UInventoryWidgetManeger>(GetWorld(),InventoryWidgetManegerClass);
		if (!InventoryWidgetInstance)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryWidgetManeger instance"));
			return;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("InventoryWidgetManeger instance created successfully"));
		}

		InventoryWidgetInstance->BeginPlay();
		InventoryWidgetInstance->SetPlayerInventory();
		InventoryWidgetInstance->UpdateInventoryUI();
		InventoryWidgetInstance->AddToViewport();
	}
	else
	{
		if (InventoryWidgetInstance)
		{
			InventoryWidgetInstance->RemoveFromParent();
		}
	}
}