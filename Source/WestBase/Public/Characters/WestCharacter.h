// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "WestCharacter.generated.h"

class AItem;
class UBoxComponent;

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
	
protected:

	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSelectBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintCallable)
	void InteractPressed();

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
	AItem* OverlappingItem;
	

};
