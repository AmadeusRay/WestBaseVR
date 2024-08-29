// Fill out your copyright notice in the Description page of Project Settings.


#include "WPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Characters/WestCharacter.h"

void AWPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	// Store a reference to the Player's Pawn
	PlayerCharacter = Cast<AWestCharacter>(aPawn);
	checkf(PlayerCharacter, TEXT("WPlayerController derived class only posses WPlayerController derived pawns"));

	// Get A reference to the EnhancedInputComponent.
	EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	checkf(EnhancedInputComponent, TEXT("Unable to get reference to the EnhancedInputComponent."))

	// Get the local player subsystem.
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	checkf(InputSubsystem, TEXT("Unable to get reference to the EnhancedInputLocalPlayerSubsystem"));

	//Wipe existing mappings, and add our mapping.
	checkf(InputMappingContext, TEXT("InputMappingContent was not sepcificed"));
	InputSubsystem->ClearAllMappings();
	InputSubsystem->AddMappingContext(InputMappingContext, 0);
	
	
	//Bind the input actions.
	//Only attempt to bind if a valid value was provided.
	if (ActionMove)
	{
		EnhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this,
			&AWPlayerController::HandleMove);
	}

		if (ActionLook)
		{
			EnhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this,
				&AWPlayerController::HandleLook);
		}
	
	if (ActionJump)
	{
		EnhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this,
			&AWPlayerController::HandleJump);
	}
}

void AWPlayerController::OnUnPossess()
{
	// Unbind things here...
	EnhancedInputComponent->ClearActionBindings();

	//Call the parent method, in case it needs to do anything
	Super::OnUnPossess();
}

void AWPlayerController::HandleLook(const FInputActionValue& InputActionValue)
{
	// Input is Vector2d
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// add yaw and pitch input to controller
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y);
}

void AWPlayerController::HandleMove(const FInputActionValue& InputActionValue)
{
	// Value is a Vector2D
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();

	// Add movement to the Player's Character Pawn
	if (PlayerCharacter)
	{
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorForwardVector(), MovementVector.Y);
		PlayerCharacter->AddMovementInput(PlayerCharacter->GetActorRightVector(), MovementVector.X);
	}
}

void AWPlayerController::HandleJump()
{
	// Input is 'Digital' (value not used here)

	// Make the Player's Character Pawn jump, disabling crouch if it was active
	if (PlayerCharacter)
	{
		PlayerCharacter->UnCrouch();
		PlayerCharacter->Jump();
	}
}
