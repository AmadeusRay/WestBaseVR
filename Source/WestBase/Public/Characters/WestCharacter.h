// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "WestCharacter.generated.h"

class AItem;
class UBoxComponent;
//
class UInputComponent;
//

UCLASS()
class WESTBASE_API AWestCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWStatlineComponent* Statline;
	
public:
	
	AWestCharacter();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; 
	
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }  

	UPROPERTY(VisibleAnywhere, Category = "Selection Properties")
	UBoxComponent* SelectionBox;
	
	//
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Interaction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InteractAction;

	/** Interaction Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* StopInteractAction;
	//
	
protected:
	virtual void BeginPlay() override;

	void PlayerJump();
	bool CanJump() const;
	void HasJumped();

	//
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for interact input */
	UFUNCTION(BlueprintCallable)
	void Interact();

	UFUNCTION(BlueprintCallable)
	void StopInteract();
	
	UFUNCTION()
	void OnSelectBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	USceneComponent* SelectTraceStart;

	UPROPERTY(BlueprintReadOnly, Category="Interaction")
	USceneComponent* SelectTraceEnd;

	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Interaction")
	float TraceDistance;

	UFUNCTION(BlueprintNativeEvent)
	void TraceForward();
	void TraceForward_Implementation();
	*/
	
private:
	UPROPERTY(VisibleInstanceOnly, Category = "Selection Properties")
	AItem* OverlappingItem

};
