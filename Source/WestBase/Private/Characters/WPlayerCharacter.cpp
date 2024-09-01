// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Characters/WestCharacter.h"

class UInputComponent;

AWPlayerCharacter::AWPlayerCharacter()
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
	//Mesh1P->bCastDynamicShadow = false;
	//Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AWPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	//
}

void AWPlayerCharacter::Move(const FInputActionValue& Value)
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

void AWPlayerCharacter::Look(const FInputActionValue& Value)
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

void AWPlayerCharacter::playerJump()
{
	if(AWestCharacter::canJump())
	{
		AWestCharacter::hasJumped();
	}
}

void AWPlayerCharacter::sprintOn()
{
	setSprinting(true);
}

void AWPlayerCharacter::sprintOff()
{
	setSprinting(false);
}

void AWPlayerCharacter::sneakOn()
{
	SetSneaking(true);
}

void AWPlayerCharacter::sneakOff()
{
	SetSneaking(false);
}

void AWPlayerCharacter::Interact()
{
	//bPressedInteract = true; // not yet done
	//InteractKeyHoldTime = 0.0f;
}

void AWPlayerCharacter::StopInteract()
{
	//bPressedInteract = false; // not yet done
	//ResetInteractstate(0);
}

// Called to bind functionality to input
void AWPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)  // needs to be in player character
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AWestCharacter::InteractPressed);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
	
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AWPlayerCharacter::playerJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AWPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AWPlayerCharacter::sprintOn);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AWPlayerCharacter::sprintOff);
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Started, this, &AWPlayerCharacter::sneakOn);
		EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Completed, this, &AWPlayerCharacter::sneakOff);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AWPlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AWPlayerCharacter::Interact);
	}
}