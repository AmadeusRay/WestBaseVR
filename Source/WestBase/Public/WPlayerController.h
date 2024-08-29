

#pragma once

#include "CoreMinimal.h"
#include "Bird.h"
#include "GameFramework/PlayerController.h"
#include "WPlayerController.generated.h"

class UInputMappingContext;
class UEnhancedInputComponent;
class AWestCharacter;

UCLASS(Abstract)
class WESTBASE_API AWPlayerController : public APlayerController
{
public:
	// The Input Action to map to movement.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionMove = nullptr;

	// The Input Action to map to looking around.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	UInputAction* ActionJump = nullptr;

	// The Input Mapping Context
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Player Input|Character Movement")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;
	
protected:
	void HandleLook(const FInputActionValue& InputActionValue);
	void HandleMove(const FInputActionValue& InputActionValue);
	void HandleJump();
	
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	
	
private:
	//  Used to store a reference to the Inputcomponent cast to an EnhancedInputComponent.
	UPROPERTY()
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = nullptr;

	// Used to store a reference to the pawn we are controlling.
	UPROPERTY()
	TObjectPtr<AWestCharacter> PlayerCharacter = nullptr;
	
	GENERATED_BODY()
	
};
