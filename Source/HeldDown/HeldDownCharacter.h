// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "Inventory.h"
#include "HeldDownCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UInventoryWidgetManeger;
class UUserWidget;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AHeldDownCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ActionButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenMainMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;
	
public:
	AHeldDownCharacter();

	virtual void GetActionButtonInfo(AActor*& ItemViewed, bool& IsPressed);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool ShouldDisplayStatUI = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool ShouldOpenMainMenue = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool CanLook = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Stats")
	bool ShouldOpenInventory = false;

	APlayerController* PlayerController;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	AActor* AactorViewed = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	bool IsActionButtonPressed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TSubclassOf<UInventoryWidgetManeger> InventoryWidgetManegerClass;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	UInventoryWidgetManeger* InventoryWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MainMenu)
	TSubclassOf<UUserWidget> MainMenueParentClass;

	UPROPERTY(BlueprintReadOnly, Category = MainMenu)
	UUserWidget* MainMenueInstance;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called when action button is pressed */
	void ActionButtonPressed(const FInputActionValue& Value);

	/** Called when action button is released */
	void ActionButtonReleased(const FInputActionValue& Value);

	/** get what player is viewing */
	AActor* ItemViewed(float ViewDistance);



protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BeginPlay() override;
	// End of APawn interface

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* MainMenuWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StatUIWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UUserWidget* StatUIWidgetInstance;

	UFUNCTION( Category = "MyEvents")
	void OpenMainMenu();

	UPROPERTY(BlueprintReadOnly, Category = "Inventory")
	AInventory* PlayerInventory;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInventory();


public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	float GetHealth();
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	float GetHunger();
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void SetHealth(float NewHealth);
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void SetHunger(float NewHunger);
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void SetShouldDisplayStatUI(bool NewShouldDisplayStats);
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	bool GetShouldDisplayStatUI();
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	void SetCanLook(bool NewCanLook);
	UFUNCTION(BlueprintCallable, Category = "Player Stats")
	bool GetCanLook();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	AInventory* GetPlayerInventory();


	

	

	

};

