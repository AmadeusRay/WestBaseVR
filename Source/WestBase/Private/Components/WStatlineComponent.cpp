// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WStatlineComponent.h"

void UWStatlineComponent::TickStats(const float& DeltaTime)
{
	TickStamina(DeltaTime);
	TickHunger(DeltaTime);
	TickThirst(DeltaTime);
	if(Thirst.GetCurrent() <= 0.0 || Hunger.GetCurrent() <= 0.0)
	{
		return;
	}
	Health.TickStat(DeltaTime);
}

void UWStatlineComponent::TickStamina(const float& DeltaTime)
{
	if(currentStaminaExhaustion > 0.0)
	{
		currentStaminaExhaustion-=DeltaTime;
		return;
	}
	
	if(bIsSprinting&&IsValidSprinting())
	{
		Stamina.TickStat(0-abs(DeltaTime*sprintCostMultiplier));
		if(Stamina.GetCurrent() <= 0.0)
		{
			setSprinting(false);
			currentStaminaExhaustion = secondsStaminaExhaustion;
		}
		return;
	}

	Stamina.TickStat(DeltaTime);
}

void UWStatlineComponent::TickHunger(const float& DeltaTime)
{
	if(Hunger.GetCurrent()  <= 0.0)
	{
		Health.Adjust(0- abs(starveHealthDamagePerSecond * DeltaTime));
		return;
	}
	Hunger.TickStat(DeltaTime);

}

void UWStatlineComponent::TickThirst(const float& DeltaTime)
{
	if(Thirst.GetCurrent() <= 0.0)
	{
		Health.Adjust(0- abs(dehydrateHealthDamagePerSecond* DeltaTime));
		return;
	}
	Thirst.TickStat(DeltaTime);

}

bool UWStatlineComponent::IsValidSprinting()
{
	return owningCharMoveComp->Velocity.Length() > walkSpeed && !owningCharMoveComp->IsFalling();
}


UWStatlineComponent::UWStatlineComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWStatlineComponent::BeginPlay()
{
	Super::BeginPlay();
	owningCharMoveComp->MaxWalkSpeed = walkSpeed;
}

void UWStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

void UWStatlineComponent::SetMoveCompRef(UCharacterMovementComponent* Comp)
{
	owningCharMoveComp = Comp;
}

float UWStatlineComponent::getStatPercentile(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:
		return Health.Percentile();
	case ECoreStat::CS_HUNGER:
		return Hunger.Percentile();
	case ECoreStat::CS_STAMINA:
		return Stamina.Percentile();
	case ECoreStat::CS_THIRST:
		return Thirst.Percentile();
	default:
		//Log invalid stat
		break;
	}
	return -1; // using -1 to signify it's a default error.
}

bool UWStatlineComponent::canSprint() const
{
	return Stamina.GetCurrent() > 0.0;
}

void UWStatlineComponent::setSprinting(const bool& IsSprinting)
{
	bIsSprinting = IsSprinting;
	if(bIsSneaking && !bIsSprinting)
	{
		return;
	}
	bIsSneaking = false;
	owningCharMoveComp->MaxWalkSpeed = bIsSprinting ? sprintSpeed : walkSpeed;
}

void UWStatlineComponent::setSneaking(const bool& IsSneaking)
{
	bIsSneaking = IsSneaking;
	if(bIsSprinting && !bIsSneaking)
	{
		return;
	}
	bIsSprinting = false;
	owningCharMoveComp->MaxWalkSpeed = bIsSneaking ? sneakSpead : walkSpeed;
}

bool UWStatlineComponent::canJump()
{
	return Stamina.GetCurrent() >= jumpCost;
}

void UWStatlineComponent::hasJumped()
{
	Stamina.Adjust(0-jumpCost);
}
