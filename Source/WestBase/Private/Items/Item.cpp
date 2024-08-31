// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "WestBase/DebugMacros.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "IDetailTreeNode.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/WestCharacter.h"

#define THIRTY 30

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	//Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	PickupWidget->SetupAttachment(RootComponent);

	

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	//Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	//Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItem::TransformedCos()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItem::RotateItem(float DeltaTime, float RateSpeed)
{
	AddActorLocalRotation(FRotator(0, RateSpeed*DeltaTime, 0));
}

/*

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AWestCharacter* WestCharacter = Cast<AWestCharacter>(OtherActor);
	if (!WestCharacter)
	{
		return;
	}
	
	if (WestCharacter)
	{
		UBoxComponent* BoxCollision = WestCharacter->FindComponentByClass<UBoxComponent>();
		
		//if (BoxCollision==WestCharacter->SelectionBox)
		if (OtherComp && OtherComp == WestCharacter->SelectionBox)
		{
			WestCharacter->SetOverlappingItem(this);
			
			UMaterialInterface* OverlayMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Material/Outlne/M_Outline.M_Outline"));
			if (OverlayMaterial && ItemMesh) // Ensure both are valid
				{
			ItemMesh->SetOverlayMaterial(OverlayMaterial);
				}
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWestCharacter* WestCharacter = Cast<AWestCharacter>(OtherActor);
	
	if (WestCharacter)
	{
		WestCharacter->SetOverlappingItem(nullptr);
		if (ItemMesh) // Ensure ItemMesh is valid
		{
			ItemMesh->SetOverlayMaterial(nullptr);
		}
	}
}
*/

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
}

