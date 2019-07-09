// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Engine/World.h"
#include "Public/EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Tags.h"
#include "GameFramework/Actor.h"
#include "Classes/Kismet/KismetMathLibrary.h"



// Sets default values, create MCRoot and VRCamera to MyPawn.
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Auto possess player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create root component
	MCRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MCRoot"));
	SetRootComponent(MCRoot);

	// Create camera component
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	// Spawn all the dynamic objects, but hide them
	SpawnDynamicObjects();

	// Spawn all the Articulated Objects
	SpawnArticulatedObjects();

	// Bind overlapping events
	SMFrustum->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPawn::OnOverlapBeginBody);
	SMFrustum->GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyPawn::OnOverlapEndBody);
	
	// Start Bind input
	Start();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Teleport the frustum every frame
	Frustum->SetActorLocation(VRCamera->GetComponentLocation(), false, (FHitResult*)nullptr, ETeleportType::None);
	Frustum->SetActorRotation(VRCamera->GetComponentRotation(), ETeleportType::None);
}

// Called to bind functionality to input
void AMyPawn::Start()
{
	UE_LOG(LogTemp, Warning, TEXT("start"));
	//Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UInputComponent* IC = PC->InputComponent)
		{
			IC->BindAction("UpdatePerception", EInputEvent::IE_Pressed, this, &AMyPawn::UpdateView);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s::%d No Input Component found.."), *FString(__func__), __LINE__);
			return;
		}
	}
	//InputComponent->BindAction("UpdatePerception", EInputEvent::IE_Pressed, this, &AMyPawn::UpdateView);


}

// Update the perceivedItems' location 
void AMyPawn::UpdateView()
{
	//UE_LOG(LogTemp, Warning, TEXT("my pawn works"));

	// Broadcast starting of grasp event
	//OnUpdatePerception.Broadcast(GetWorld()->GetTimeSeconds());

	// Clean up the CurrentVisibleMeshes Array every time update view.
	CurrentVisibleMeshes.Empty();
	// Return the objects that are inside the frustum
	TArray<AActor*> OverlappingActors;
	SMFrustum->GetOverlappingActors(OverlappingActors,TSubclassOf<AStaticMeshActor>());
	// Check overlapping objects are actually perceived by the user, specially for invisible real meshes.
	for (int32 Index = 0; Index != OverlappingActors.Num(); ++Index)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Perceived Overlapping actors:  %s"), *OverlappingActors[Index]->GetName());
		// Get the corrresponding visual meshes by Find Reference on TMap.
		//if (RealToVisual.Contains(Cast<AStaticMeshActor>(OverlappingActors[Index])))
		if(OverlappingActors[Index]->ActorHasTag("RoboWorld;ObjectType,Dynamic;")|| OverlappingActors[Index]->ActorHasTag("RoboWorld;ObjectType,Articulated;"))
		{
			// Do LineTraceSinglebyChannel, get perceived objects without occluded by other actors.
			FVector StartLocation = VRCamera->GetComponentLocation();
			FVector EndLocation = OverlappingActors[Index]->GetActorLocation();
			
			FHitResult OutHit;
			FHitResult OutHitMin;
			FHitResult OutHitMax;
			ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel3;
			FCollisionQueryParams CollisionParams;

			
			FVector Min;
			FVector Max;
			(Cast<AStaticMeshActor>(OverlappingActors[Index]))->GetStaticMeshComponent()->GetLocalBounds(Min, Max);
			FVector EndLocationMin = UKismetMathLibrary::TransformLocation((Cast<AStaticMeshActor>(OverlappingActors[Index]))->GetStaticMeshComponent()->GetComponentTransform(), Min);
			FVector EndLocationMax = UKismetMathLibrary::TransformLocation((Cast<AStaticMeshActor>(OverlappingActors[Index]))->GetStaticMeshComponent()->GetComponentTransform(), Max);
			
			CollisionParams.AddIgnoredActor(OverlappingActors[Index]);
			
			

			bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, TraceChannel, CollisionParams);
			bool bHitMin = GetWorld()->LineTraceSingleByChannel(OutHitMin, StartLocation, EndLocationMin, TraceChannel, CollisionParams);
			bool bHitMax = GetWorld()->LineTraceSingleByChannel(OutHitMax, StartLocation, EndLocationMax, TraceChannel, CollisionParams);
			if (!bHit || !bHitMin || !bHitMax)
			{
				// Save perceived real meshes in to CurrentVisibleMeshes Array
				CurrentVisibleMeshes.Emplace(Cast<AStaticMeshActor>(OverlappingActors[Index]));
				AStaticMeshActor* OverlappingActorPtr = Cast<AStaticMeshActor>(OverlappingActors[Index]);
				AStaticMeshActor* VisualCorrespondingMesh = RealToVisual.FindRef(OverlappingActorPtr);
				// Update the position and location of the visual meshes.
				FVector Location = OverlappingActors[Index]->GetActorLocation() + FVector(FMath::FRandRange(0.0f,0.5f), FMath::FRandRange(0.0f, 0.5f),0.0f);
				FRotator Rotation = OverlappingActors[Index]->GetActorRotation();
				VisualCorrespondingMesh->SetActorLocation(Location, false, (FHitResult*)nullptr, ETeleportType::None);
				VisualCorrespondingMesh->SetActorRotation(Rotation);
				VisualCorrespondingMesh->SetActorHiddenInGame(false);
				UE_LOG(LogTemp, Warning, TEXT("Perceived Overlapping actors:  %s"), *OverlappingActors[Index]->GetName());
				//UE_LOG(LogTemp, Warning, TEXT("Perceived Overlapping actors:  %s"), *VisualCorrespondingMesh->GetName());
			}
			

			/*if (bHit)
			{
				UE_LOG(LogTemp, Warning, TEXT("NonPerceived Overlapping actors:  %s"), *OverlappingActors[Index]->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Collision actors:  %s"), *OutHit.Actor->GetName());
				UE_LOG(LogTemp, Warning, TEXT("center world location:  %s"), *EndLocation.ToString());
			}
			if (bHitMin)
			{
				UE_LOG(LogTemp, Warning, TEXT("NonPerceived Overlapping actors:  %s"), *OverlappingActors[Index]->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Min Collision actors:  %s"), *OutHitMin.Actor->GetName());
				UE_LOG(LogTemp, Warning, TEXT("min world location:  %s"), *EndLocationMin.ToString());
			}
			if (bHitMax)
			{
				UE_LOG(LogTemp, Warning, TEXT("NonPerceived Overlapping actors:  %s"), *OverlappingActors[Index]->GetName());
				UE_LOG(LogTemp, Warning, TEXT("Max Collision actors:  %s"), *OutHitMax.Actor->GetName());
				UE_LOG(LogTemp, Warning, TEXT("max world location:  %s"), *EndLocationMax.ToString());
			}*/
		}
	}

	

		// Find visual meshes are perceived by the user, but real mesh are ouside the view, set visual mesh to be invisible.
	for (int32 Index = 0; Index != OverlappingActors.Num(); ++Index)
	{
		AStaticMeshActor* RealMeshPtr = Cast<AStaticMeshActor>(OverlappingActors[Index]);
		if (RealMeshPtr->GetStaticMeshComponent()->ComponentHasTag("PerceivedDynamicItems")|| RealMeshPtr->GetName().Contains(FString("VisualSM_handle_2")))
		{
			UE_LOG(LogTemp, Warning, TEXT("visual mesh overlap:  %s"), *OverlappingActors[Index]->GetName());
			
			AStaticMeshActor* RealCorrespondingActor = VisualToReal.FindRef(RealMeshPtr);
			if (!CurrentVisibleMeshes.Contains(RealCorrespondingActor))
			{
				UE_LOG(LogTemp, Warning, TEXT("visual mesh lost connection:  %s"), *OverlappingActors[Index]->GetName());
				// Do LineTraceSinglebyChannel, get perceived objects without occluded by other actors.
				FVector StartLocation = VRCamera->GetComponentLocation();
				FVector EndLocation = OverlappingActors[Index]->GetActorLocation();
				FHitResult OutHit;
				FHitResult OutHitMin;
				FHitResult OutHitMax;
				ECollisionChannel TraceChannel = ECollisionChannel::ECC_GameTraceChannel4;
				FCollisionQueryParams CollisionParams;
				FVector Min;
				FVector Max;
				(Cast<AStaticMeshActor>(OverlappingActors[Index]))->GetStaticMeshComponent()->GetLocalBounds(Min, Max);
				FVector EndLocationMin = UKismetMathLibrary::TransformLocation((Cast<AStaticMeshActor>(OverlappingActors[Index]))->GetStaticMeshComponent()->GetComponentTransform(), Min);
				FVector EndLocationMax = UKismetMathLibrary::TransformLocation((Cast<AStaticMeshActor>(OverlappingActors[Index]))->GetStaticMeshComponent()->GetComponentTransform(), Max);


				//CollisionParams.AddIgnoredActor(Cast<AActor>(RealCorrespondingActor));
				CollisionParams.AddIgnoredActor(OverlappingActors[Index]);
				bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, TraceChannel, CollisionParams);
				bool bHitMin = GetWorld()->LineTraceSingleByChannel(OutHitMin, StartLocation, EndLocationMin, TraceChannel, CollisionParams);
				bool bHitMax = GetWorld()->LineTraceSingleByChannel(OutHitMax, StartLocation, EndLocationMax, TraceChannel, CollisionParams);
				if (!bHit || !bHitMin || !bHitMax)
				{					
					// Find the visual meshes lost connect to the real mesh, set it to be invisible.						
					OverlappingActors[Index]->SetActorHiddenInGame(true);
					//UE_LOG(LogTemp, Warning, TEXT("visual mesh lost connection:  %s"), *OverlappingActors[Index]->GetName());
					
				}
			}
			
			
		}
		
		

	}
	
	

	
	


	
}

// Spawn Dynamic Objects at the begin play but set them to be invisible at the beginning
void AMyPawn::SpawnDynamicObjects()
{
	UWorld* const World = GetWorld();
	DynamicActors = FTags::GetActorsWithKeyValuePair(GetWorld(), "RoboWorld", "ObjectType", "Dynamic");
	for (int32 Index = 0; Index != DynamicActors.Num(); ++Index)
	{
		// Set real objects to invisible
		DynamicActors[Index]->SetActorHiddenInGame(true);
		// Cast AActor to AStaticMeshActor
		DynamicStaticMeshActors.Add(Cast<AStaticMeshActor>(DynamicActors[Index]));
		// Make a copy of the static mesh actor
		FVector location = DynamicStaticMeshActors[Index]->GetActorLocation();
		FRotator rotation = DynamicStaticMeshActors[Index]->GetActorRotation();		
		AStaticMeshActor* SpawnedActor1 = (AStaticMeshActor*)World->SpawnActor(DynamicStaticMeshActors[Index]->StaticClass(), &location, &rotation);
		Cast<AActor>(SpawnedActor1)->SetActorLabel("Visual" + DynamicStaticMeshActors[Index]->GetFName().ToString());
		SpawnedActor1->GetStaticMeshComponent()->SetStaticMesh(DynamicStaticMeshActors[Index]->GetStaticMeshComponent()->GetStaticMesh());
		SpawnedActor1->SetActorHiddenInGame(true);
		SpawnedActor1->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("PerceivedItems"));
		SpawnedActor1->GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
		SpawnedActor1->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		SpawnedActor1->GetStaticMeshComponent()->ComponentTags.Add("PerceivedDynamicItems");
		AllVisualObjects.Emplace(SpawnedActor1);
		// Fill in TMap
		RealToVisual.Emplace(DynamicStaticMeshActors[Index], SpawnedActor1);
		VisualToReal.Emplace(SpawnedActor1, DynamicStaticMeshActors[Index]);
	}
	
}

void AMyPawn::SpawnArticulatedObjects()
{
	UWorld* const World = GetWorld();
	ArticulatedActors = FTags::GetActorsWithKeyValuePair(GetWorld(), "RoboWorld", "ObjectType", "Articulated");
	for (int32 Index = 0; Index != ArticulatedActors.Num(); ++Index)
	{
		// Set real objects to invisible
		ArticulatedActors[Index]->SetActorHiddenInGame(true);
		// Cast AActor to AStaticMeshActor
		ArticulatedStaticMeshActors.Add(Cast<AStaticMeshActor>(ArticulatedActors[Index]));
		// Make a copy of the static mesh actor
		FVector location = ArticulatedStaticMeshActors[Index]->GetActorLocation();
		FRotator rotation = ArticulatedStaticMeshActors[Index]->GetActorRotation();
		AStaticMeshActor* SpawnedActor2 = (AStaticMeshActor*)World->SpawnActor(ArticulatedStaticMeshActors[Index]->StaticClass(), &location, &rotation);
		SpawnedActor2->SetActorLabel("Visual" + ArticulatedStaticMeshActors[Index]->GetFName().ToString());
		SpawnedActor2->GetStaticMeshComponent()->SetStaticMesh(ArticulatedStaticMeshActors[Index]->GetStaticMeshComponent()->GetStaticMesh());
		SpawnedActor2->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("PerceivedItems"));
		SpawnedActor2->GetStaticMeshComponent()->SetGenerateOverlapEvents(true);
		SpawnedActor2->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
		SpawnedActor2->GetStaticMeshComponent()->ComponentTags.Add("PerceivedArticulatedItems");
		AllVisualObjects.Emplace(SpawnedActor2);
		// Fill in TMap
		RealToVisual.Emplace(ArticulatedStaticMeshActors[Index], SpawnedActor2);
		VisualToReal.Emplace(SpawnedActor2, ArticulatedStaticMeshActors[Index]);

	}

}

// check overlapping begin for controller enters in the frustum
void AMyPawn::OnOverlapBeginBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->GetName().Contains("WSG"))
		{
			//UE_LOG(LogTemp, Warning, TEXT("controller enter %s"), *OtherActor->GetName());
			OtherComp->SetRenderCustomDepth(false);
		}
	}
}

// check overlapping end for controller enters in the frustum
void AMyPawn::OnOverlapEndBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->GetName().Contains("WSG"))
		{
			//UE_LOG(LogTemp, Warning, TEXT("controller highlight %s"), *OtherActor->GetName());
			OtherComp->SetRenderCustomDepth(true);
		}
	}
}





