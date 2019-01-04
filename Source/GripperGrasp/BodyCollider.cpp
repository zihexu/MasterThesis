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

	// ...
}


// Called when the game starts
void UBodyCollider::BeginPlay()
{
	Super::BeginPlay();

	// Get the player camera 
	TArray<UCameraComponent*> Comps;

	MyPawn->GetComponents(Comps);
	if (Comps.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("FIND COMPONENTS %s"),*Comps[0]->GetName());
		UCameraComponent* MyCamera = Comps[0];
	}
	//UCameraComponent* MyCamera = CastChecked<UCameraComponent>(MyPawn->FindComponentByClass(UCameraComponent::StaticClass()));
	//if (GEngine) GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->bEnableFading = true;

	//set the default value for the indicator
	bShowIndicator = 0;
	bDarkView = 0;
	// Set default state for Body Collider
	if (UStaticMeshComponent* StaticMeshComp = BodyCollider->GetStaticMeshComponent())
	{

		StaticMeshComp->SetMobility(EComponentMobility::Movable);
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetEnableGravity(false);
		StaticMeshComp->SetCollisionProfileName(TEXT("Trigger"));

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
	//Teleport Body Collider, follow camera's location
	if (UStaticMeshComponent* StaticMeshComp = BodyCollider->GetStaticMeshComponent())
	{
		StaticMeshComp->SetWorldLocation((FVector(GetComponentLocation().X, GetComponentLocation().Y, 70.0)), false, (FHitResult*)nullptr, ETeleportType::None);

	}

	

	
	if (bShowIndicator)
	{
		//Teleport the arrow indicators at runtime
		for (int32 Index = 0; Index != ArrowIndicators.Num(); ++Index)
		{
			if (ArrowIndicators[Index]&& OverlappingActors[Index])
			{
				ArrowIndicators[Index]->SetActorHiddenInGame(false);
				ArrowIndicators[Index]->SetActorLocation((GetComponentLocation() + GetForwardVector() * 50), false, (FHitResult*)nullptr, ETeleportType::None);
				FVector LookatDirection = OverlappingActors[Index]->GetActorLocation() - (ArrowIndicators[Index]->GetActorLocation());
				FRotator lookAtRotator = FRotationMatrix::MakeFromX(LookatDirection).Rotator();
				ArrowIndicators[Index]->SetActorRotation(lookAtRotator);
		
				break;
			}
		}

	}
	

	// Dark the camera view
	if (MyCamera)
	{
		UE_LOG(LogTemp, Warning, TEXT("FIND THE CAMERA"));
		if (bDarkView)
		{
			FPostProcessSettings PostPro;

			PostPro.bOverride_ColorGain = true;
			PostPro.ColorGain = FVector4(0.0f, 0.0f, 0.0f, 1.0f);

			MyCamera->PostProcessSettings = PostPro;
		}
		else if (!bDarkView)
		{
			FPostProcessSettings PostPro;

			PostPro.bOverride_ColorGain = true;
			PostPro.ColorGain = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
			MyCamera->PostProcessSettings = PostPro;
		}
	}
	

}

void UBodyCollider::OnOverlapBeginBody(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor!=NULL)
	{
		if (SpawnArrowIndicator)
		{
			if (OtherActor->ActorHasTag("Collider"))
			{
				i = i + 1;
				bShowIndicator = 1;
				bDarkView = 1;
				OverlapNum = OverlapNum + 1;
				OverlappingActors.Emplace(OtherActor);
				OtherComp->SetRenderCustomDepth(true);
				//UE_LOG(LogTemp, Warning, TEXT("spawn arrow indicator"));
				//spawn the arrow indicator
				UWorld* World = GetWorld();
				if (World)
				{
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = GetOwner();

					FVector SpawnLocation = (GetComponentLocation()+OtherActor->GetActorLocation())*0.5;
					FRotator SpawnRotation = GetComponentRotation();
					// Spawn the arrow indicator
					ArrowIndicators.Emplace(World->SpawnActor<ASpawnActor>(SpawnArrowIndicator, SpawnLocation, SpawnRotation, SpawnParams));
						

				}
			}
		
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
			bDarkView = 0;
		}
		for (int32 Index = 0; Index != OverlappingActors.Num(); ++Index)
		{
			if (OtherActor == OverlappingActors[Index])
			{
				ArrowIndicators[Index]->Destroy();
				OverlappingActors[Index] = nullptr;
				OtherComp->SetRenderCustomDepth(false);
			}
				
		}
		
	}
}

