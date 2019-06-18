// Fill out your copyright notice in the Description page of Project Settings.

#include "BodyCollider.h"
#include "Classes/Engine/World.h "
#include "Classes/Camera/CameraComponent.h"
#include "SpawnActor.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine.h"



// Sets default values for this component's properties
UBodyCollider::UBodyCollider()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	/*ViveTracker1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ViveTracker1"));
	ViveTracker1->MotionSource = FName(TEXT("Special_1"));*/
	//ViveTracker1->SetupAttachment();

	ViveTracker2 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ViveTracker2"));
	ViveTracker2->MotionSource = FName(TEXT("Special_2"));
	//ViveTracker2->SetupAttachment(RootComponent);
	// ...
}


// Called when the game starts
void UBodyCollider::BeginPlay()
{
	Super::BeginPlay();

	// Spawn ArrowIndicator
	SpawnArrowIndicatorFunction();

	//set the default value for the indicator
	bShowIndicator = 0;
	
	// Set default state for Body Collider
	if (UStaticMeshComponent* StaticMeshComp = BodyCollider->GetStaticMeshComponent())
	{

		StaticMeshComp->SetMobility(EComponentMobility::Movable);
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetEnableGravity(false);
		//StaticMeshComp->SetCollisionProfileName(TEXT("Trigger"));

		//Register Events for LeftSphereVC

		BodyCollider->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &UBodyCollider::OnOverlapBeginBody);
		BodyCollider->GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &UBodyCollider::OnOverlapEndBody);


	}
	// ...
	
}


// Called every frame
void UBodyCollider::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//Teleport Body Collider, follow Vive trachers' location, use two trackers to balence the center point
	if (UStaticMeshComponent* StaticMeshComp = BodyCollider->GetStaticMeshComponent())
	{
		FVector TeleportLocation = ViveTracker2->GetComponentLocation() + FVector(0.0f,OffsetValue,0.0f);
		StaticMeshComp->SetWorldLocation((FVector(TeleportLocation.X, TeleportLocation.Y, 32.5f)), false, (FHitResult*)nullptr, ETeleportType::None);
		StaticMeshComp->SetWorldRotation(FRotator(0, ViveTracker2->GetComponentRotation().Yaw, 0), false, (FHitResult*)nullptr, ETeleportType::None);
		//UE_LOG(LogTemp, Warning, TEXT("get tracker location: %s"), *ViveTracker2->GetComponentLocation().ToString());
	}

	

	
	if (OverlapNum!=0)
	{
		//Teleport the arrow indicators at runtime
		for (int32 Index = 0; Index != OverlappingActors.Num(); ++Index)
		{
			if (ArrowIndicator&& OverlappingActors[Index])
			{
				ArrowIndicator->SetActorHiddenInGame(false);
				//Teleport the arrow indicator to the forward location of player's camera
				ArrowIndicator->SetActorLocation((GetComponentLocation() + GetForwardVector() * 50), false, (FHitResult*)nullptr, ETeleportType::None);
				//Keep the arrow indicator's rotation to look at the colliding actors
				FVector LookatDirection = OverlappingActors[Index]->GetActorLocation() - (ArrowIndicator->GetActorLocation());
				//FVector LookatDirection = HitPosition[Index] - (ArrowIndicator->GetActorLocation());

				FRotator lookAtRotator = FRotationMatrix::MakeFromX(LookatDirection).Rotator();
				ArrowIndicator->SetActorRotation(lookAtRotator);
		
				break;
			}
		}

	}
	
	
	

}

void UBodyCollider::OnOverlapBeginBody(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor!=NULL)
	{
		if (SpawnArrowIndicator)
		{
			//if (OtherActor->ActorHasTag("RoboWorld;ObjectType,Articulated;") || OtherActor->ActorHasTag("RoboWorld;ObjectType,Static;"))
			
				// play the colliding sound
				UGameplayStatics::PlaySoundAtLocation(this, CollideSound, OtherActor->GetActorLocation());

				// save the overlapping event hit position to be used for the arrow to point
				HitPosition.Emplace(SweepResult.Location);


				i = i + 1;
				//bShowIndicator = 1;
				//bDarkView = 1;
				OverlapNum = OverlapNum + 1;
				OverlappingActors.Emplace(OtherActor);
				OtherComp->SetRenderCustomDepth(true);
				//UE_LOG(LogTemp, Warning, TEXT("spawn arrow indicator"));
				
		
		}
	}
}

void UBodyCollider::OnOverlapEndBody(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != NULL)
	{
		OverlapNum = OverlapNum - 1;
		if (OverlapNum == 0)
		{
			ArrowIndicator->SetActorHiddenInGame(true);
		}
		for (int32 Index = 0; Index != OverlappingActors.Num(); ++Index)
		{
			if (OtherActor == OverlappingActors[Index])
			{
				//OverlappingActors.Remove(OtherActor);
				OverlappingActors[Index] = nullptr;
				OtherComp->SetRenderCustomDepth(false);
				//UE_LOG(LogTemp, Warning, TEXT("collide with object %s"), *OverlappingActors[Index]->GetName());
			}
				
		}
		
	}
}

void UBodyCollider::SpawnArrowIndicatorFunction()
{
	//spawn the arrow indicator
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();

		FVector SpawnLocation = GetComponentLocation();

		FRotator SpawnRotation = GetComponentRotation();
		// Spawn the arrow indicator
		ArrowIndicator = World->SpawnActor<ASpawnActor>(SpawnArrowIndicator, SpawnLocation, SpawnRotation, SpawnParams);
		ArrowIndicator->SetActorHiddenInGame(true);


	}
}


