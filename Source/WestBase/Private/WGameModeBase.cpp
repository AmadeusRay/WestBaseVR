// Fill out your copyright notice in the Description page of Project Settings.


#include "WGameModeBase.h"
#include "UObject/GarbageCollectionSchema.h"


AWGameModeBase::AWGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/VR_Weapons/Pawn/Blueprints/MotionControllerPawn.uasset"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}