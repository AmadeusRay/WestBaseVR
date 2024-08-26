// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WestCharacter.h"
#include "Components/WStatlineComponent.h"
#include "Components/BoxComponent.h"
#include "Items/Item.h"
#include "Kismet/KismetSystemLibrary.h"


AWestCharacter::AWestCharacter()
{
	Statline = CreateDefaultSubobject<UWStatlineComponent>(TEXT("Statline"));
	Statline->SetMoveCompRef(GetCharacterMovement());
	
	SelectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Selection Box"));
	SelectionBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("hand_r"));
	SelectionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SelectionBox->SetCollisionResponseToAllChannels(ECR_Overlap);  // Sets all channels to overlap then the next one...
	SelectionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);// specifically sets
	//Pawn ones to ignore. Easier than individually doing it.
	
	SelectTraceStart =  CreateDefaultSubobject<USceneComponent>(TEXT("Select Trace Start"));
	SelectTraceStart->SetupAttachment(SelectionBox);
	
	SelectTraceEnd =  CreateDefaultSubobject<USceneComponent>(TEXT("Select Trace End"));
	SelectTraceEnd->SetupAttachment(SelectionBox);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//TraceDistance = 2000.0f;
}

void AWestCharacter::BeginPlay()
{
	Super::BeginPlay();

	SelectionBox->OnComponentBeginOverlap.AddDynamic(this, &AWestCharacter::OnSelectBoxOverlap);
}

void AWestCharacter::OnSelectBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = SelectTraceStart->GetComponentLocation();
	const FVector End = SelectTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult BoxHit;
	
	//UKismetSystemLibrary::BoxTraceSingle(this, Start, End, FVector(5.f, 5.f, 5.f), SelectTraceStart->GetComponentRotation(),ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::ForDuration, BoxHit, true);

	if (OtherActor && OtherActor->IsA(AItem::StaticClass()))
	{
		UStaticMeshComponent* MeshComponent = OtherActor->FindComponentByClass<UStaticMeshComponent>();
		UStaticMeshComponent* OverlappedItem = Cast<UStaticMeshComponent>(OtherActor);
		// Apply the overlay material to the item
		UMaterial* OverlayMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Game/Material/Outline/M_Outline.uasset"));
		if (OverlayMaterial)
		{
			OverlappedItem->GetStaticMesh()->SetMaterial(0, OverlayMaterial);
		}
	}
}

void AWestCharacter::InteractPressed()
{
}


/*
void AWestCharacter::TraceForward_Implementation()
{
	FVector Loc;
	FRotator Rot;
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;

	GetController()->GetPlayerViewPoint(Loc, Rot);

	FVector Start = Loc;
	FVector End = Start + (Rot.Vector() * TraceDistance);

	FCollisionParameters TraceParams;
	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, CollisionParams);

	DrawDebugLine(GetWorld(), Start, End, FColor::Purple, false, 2.0f);
	if(bHit)
	{
		
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5,5,5), FColor::Emerald, false, 2.0f);
			FName SocketName = TEXT("RightHandSocket"); // for some reason it's attaching to to the IK skeltalmesh probably set in BP. 

		if (GetMesh()->DoesSocketExist(SocketName))
		{
			Hit.GetActor()->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

			UE_LOG(LogTemp, Warning, TEXT("%s attached to %s"), *Hit.GetActor()->GetName(), *SocketName.ToString());
		}
	}
	// attachment works. You have to make sure physics is disabled before holding the gun.
}
*/

void AWestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



// Called to bind functionality to input
void AWestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AWestCharacter::InteractPressed);
}

