// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WStatlineComponent.generated.h"

UENUM(BlueprintType)
enum class ECoreStat : uint8
{
	CS_HEALTH UMETA(DisplayName="Health"),
	CS_STAMINA UMETA(DisplayName="Stamina"),
	CS_HUNGER UMETA(DisplayName="Hunger"),
	CS_THIRST UMETA(DisplayName="Thirst")
};

USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Current = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Max = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

public:
	FCoreStat()
	{
		
	};
	FCoreStat(const float& current, const float& max, const float& tick)
	{
		Current = current;
		Max = max;
		PerSecondTick = tick;
	}
	
	void TickStat(const float& DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0, Max);
	}

	void Adjust(const float& amount)
	{
		Current = FMath::Clamp(Current + amount, 0, Max);
	}

	float Percentile() const
	{
		return Current / Max;
	}

	void AdjustTick(const float& NewTick)
	{
		PerSecondTick = NewTick;
	}

	float GetCurrent() const
	{
		return Current;
	}
	
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WESTBASE_API UWStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

private:

	class UCharacterMovementComponent* owningCharMoveComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Stamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Hunger = FCoreStat (100,100, -.25);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FCoreStat Thirst = FCoreStat(100, 100, -0.25);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float sprintCostMultiplier = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float walkSpeed = 125;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float sprintSpeed = 450;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float jumpCost = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float secondsStaminaExhaustion = 5;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float currentStaminaExhaustion = 0;

	void TickStats(const float& DeltaTime);
	void TickStamina(const float& DeltaTime);
	bool IsValidSprinting();

protected:

	virtual void BeginPlay() override;

public:
	UWStatlineComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMoveCompRef(UCharacterMovementComponent* Comp);
	
	UFUNCTION(BlueprintCallable)
	float getStatPercentile(const ECoreStat Stat) const;

	UFUNCTION(BlueprintCallable)
	bool canSprint()const;

	UFUNCTION(BlueprintCallable)
	void setSprinting(const bool& IsSprinting);

	UFUNCTION(BlueprintCallable)
	bool canJump();

	UFUNCTION(BlueprintCallable)
	void hasJumped();
};
