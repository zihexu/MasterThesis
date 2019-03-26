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


	ViveTracker1 = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ViveTracker1"));
	ViveTracker1->MotionSource = FName(TEXT("Special_1"));
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

	
	UCameraComponent* MyCamera = CastChecked<UCameraComponent>(this->GetOwner()->FindComponentByClass(UCameraComponent::StaticClass()));

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
	//Teleport Body Collider, follow Vive trachers' location, use two trackers to balence the center point
	if (UStaticMeshComponent* StaticMeshComp = BodyCollider->GetStaticMeshComponent())
	{
		FVector TeleportLocation = (ViveTracker1->GetComponentLocation()+ ViveTracker2->GetComponentLocation())/2;
		StaticMeshComp->SetWorldLocation((FVector(TeleportLocation.X, TeleportLocation.Y, 70.0)), false, (FHitResult*)nullptr, ETeleportType::None);
		StaticMeshComp->SetWorldRotation(FRotator(0, ViveTracker1->GetComponentRotation().Yaw, 0)), false, (FHitResult*)nullptr, ETeleportType::None;
	}

	

	
	if (bShowIndicator)
	{
		//Teleport the arrow indicators at runtime
		for (int32 Index = 0; Index != ArrowIndicators.Num(); ++Index)
		{
			if (ArrowIndicators[Index]&& OverlappingActors[Index])
			{
				ArrowIndicators[Index]->SetActorHiddenInGame(false);
				//Teleport the arrow indicator to the forward location of player's camera
				ArrowIndicators[Index]->SetActorLocation((GetComponentLocation() + GetForwardVector() * 50), false, (FHitResult*)nullptr, ETeleportType::None);
				//Keep the arrow indicator's rotation to look at the colliding actors
				FVector LookatDirection = OverlappingActors[Index]->GetActorLocation() - (ArrowIndicators[Index]->GetActorLocation());
				FRotator lookAtRotator = FRotationMatrix::MakeFromX(LookatDirection).Rotator();
				ArrowIndicators[Index]->SetActorRotation(lookAtRotator);
		
				break;
			}
		}

	}
	
	
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
	
	// Dark the camera view
	
	

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

