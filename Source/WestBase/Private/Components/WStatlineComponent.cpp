// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WStatlineComponent.h"

void UWStatlineComponent::TickStats(const float& DeltaTime)
{
	Health.TickStat(DeltaTime);
	TickStamina(DeltaTime);
	Hunger.TickStat(DeltaTime);
	Thirst.TickStat(DeltaTime);
}

void UWStatlineComponent::TickStamina(const float& DeltaTime)
{
	if(bIsSprinting&&IsValidSprinting())
	{
	Stamina.TickStat(0-(DeltaTime*sprintCostMultiplier));
		return;
	}
	Stamina.TickStat(DeltaTime);
}

bool UWStatlineComponent::IsValidSprinting()
{
	return owningCharMoveComp->Velocity.Length() > walkSpeed && !owningCharMoveComp->IsFalling();
}

// Sets default values for this component's properties
UWStatlineComponent::UWStatlineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UWStatlineComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
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
	owningCharMoveComp->MaxWalkSpeed = bIsSprinting ? sprintSpeed : walkSpeed;
}

bool UWStatlineComponent::canJump()
{
	return Stamina.GetCurrent() >= jumpCost;
}

void UWStatlineComponent::hasJumped()
{
	Stamina.Adjust(0-jumpCost);
}
