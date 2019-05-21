// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Engine/World.h"
#include "Public/EngineUtils.h"
#include "GameFramework/PlayerController.h"
#include "Tags.h"


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

	////Bind overlapping events
	//Frustum->OnActorBeginOverlap.AddDynamic(this, &AMyPawn::OnActorBeginOverlap);
	//Frustum->OnActorBeginOverlap.AddDynamic(this, &AMyPawn::OnActorEndOverlap);
	SMFrustum->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyPawn::OnOverlapBeginBody);
	SMFrustum->GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &AMyPawn::OnOverlapEndBody);

	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("destroyed actor %s"), *ClonedObjects[Index]->GetName());
	Frustum->SetActorLocation(VRCamera->GetComponentLocation(), false, (FHitResult*)nullptr, ETeleportType::None);
	Frustum->SetActorRotation(VRCamera->GetComponentRotation(), ETeleportType::None);
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("UpdateViewTouchpad", EInputEvent::IE_Pressed, this, &AMyPawn::UpdateView);


}

void AMyPawn::UpdateView()
{
	// Return the objects that are inside the frustum
	TArray<AActor*> OverlappingActors;
	Frustum->GetOverlappingActors(OverlappingActors,TSubclassOf<AStaticMeshActor>());
	for (int32 Index = 0; Index != OverlappingActors.Num(); ++Index)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping actosr:  %s"), *OverlappingActors[Index]->GetName());
	}
	
	// Update the location of all cloned Articulated objects
	for (int32 Index = 0; Index != ClonedArticulatedObjects.Num(); ++Index)
	{
		if (OverlappingActors.Contains(ClonedArticulatedObjects[Index]))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapping Articulated actors:  %s"), *ClonedArticulatedObjects[Index]->GetName());
			AActor* Reference = ArticulatedObjects[Index];
			FVector NewLocation = Reference->GetActorLocation();
			//Teleport the cloned objects
			ClonedArticulatedObjects[Index]->SetActorLocation(NewLocation, false, (FHitResult*)nullptr, ETeleportType::None);
			//UE_LOG(LogTemp, Warning, TEXT("destroyed actor %s"), *ClonedObjects[Index]->GetName());
			ClonedArticulatedObjects[Index]->SetActorRotation(Reference->GetActorRotation());
		}
	}

	// Update the location of all cloned Dynamic objects
	for (int32 Index = 0; Index != ClonedDynamicObjects.Num(); ++Index)
		if (OverlappingActors.Contains(ClonedDynamicObjects[Index]))
		{
			UE_LOG(LogTemp, Warning, TEXT("Overlapping Dynamic actors:  %s"), *ClonedDynamicObjects[Index]->GetName());
			ClonedDynamicObjects[Index]->SetActorHiddenInGame(false);
			AActor* Reference = DynamicActors[Index];
			FVector NewLocation = Reference->GetActorLocation() + FMath::VRand() * 1;
			//Teleport the cloned objects
			ClonedDynamicObjects[Index]->SetActorLocation(NewLocation, false, (FHitResult*)nullptr, ETeleportType::None);
			//UE_LOG(LogTemp, Warning, TEXT("destroyed actor %s"), *ClonedObjects[Index]->GetName());
			ClonedDynamicObjects[Index]->SetActorRotation(Reference->GetActorRotation());
		}
	


	
}

// Spawn Dynamic Objects at the begin play but set them to be invisible at the beginning
void AMyPawn::SpawnDynamicObjects()
{
	UWorld* const World = GetWorld();
	DynamicActors = FTags::GetActorsWithKeyValuePair(GetWorld(), "RoboWorld", "ObjectType", "Dynamic");
	for (int32 Index = 0; Index != DynamicActors.Num(); ++Index)
	{
		DynamicStaticMeshActors.Add(Cast<AStaticMeshActor>(DynamicActors[Index]));

	}



	for (int32 Index = 0; Index != DynamicActors.Num(); ++Index)
	{
		DynamicActors[Index]->SetActorHiddenInGame(true);
		//UE_LOG(LogTemp, Warning, TEXT("DynamicObjects contains %s"), *ClonedObjects[Index]->GetName());
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Template = DynamicActors[Index];

		// The random error location to spawn  + FMath::VRand() * 5
		FVector WorldLocation = DynamicActors[Index]->GetActorLocation();

		FVector SpawnLocation = FVector(0,0,0);

		// The rotation to spawn
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		// The name of the spawned actor
		FName DuplicatedName = DynamicActors[Index]->GetFName();

		// Spawn the objects 
		AActor* SpawnActor = World->SpawnActor<AActor>(DynamicActors[Index]->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
		SpawnActor->SetActorLocation(WorldLocation);
		SpawnActor->SetActorRotation(DynamicActors[Index]->GetActorRotation());
		SpawnActor->SetActorHiddenInGame(true);
		SpawnActor->DisableComponentsSimulatePhysics();
		Cast<UStaticMeshComponent>(SpawnActor->GetComponentByClass(UStaticMeshComponent::StaticClass()))->SetCollisionProfileName(TEXT("OverlapAll"));
		SpawnActor->SetActorLabel(DuplicatedName.ToString() + "copy");
		ClonedDynamicObjects.Add(SpawnActor);
		UE_LOG(LogTemp, Warning, TEXT("DuplicateDynamicMesh %s"), *ClonedDynamicObjects[Index]->GetName());
	
	}
	
}

void AMyPawn::SpawnArticulatedObjects()
{
	UWorld* const World = GetWorld();
	ArticulatedObjects = FTags::GetActorsWithKeyValuePair(GetWorld(), "RoboWorld", "ObjectType", "Articulated");
	for (int32 Index = 0; Index != ArticulatedObjects.Num(); ++Index)
	{
		ArticulatedObjects[Index]->SetActorHiddenInGame(true);
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		SpawnParams.Template = ArticulatedObjects[Index];

		// The random error location to spawn  + FMath::VRand() * 5
		FVector WorldLocation = ArticulatedObjects[Index]->GetActorLocation();

		FVector SpawnLocation = FVector(0, 0, 0);

		// The rotation to spawn
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		// The name of the spawned actor
		FName DuplicatedName = ArticulatedObjects[Index]->GetFName();

		// Spawn the objects 
		AActor* SpawnActor = World->SpawnActor<AActor>(ArticulatedObjects[Index]->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
		SpawnActor->SetActorLocation(WorldLocation);
		SpawnActor->SetActorRotation(ArticulatedObjects[Index]->GetActorRotation());
		SpawnActor->SetActorHiddenInGame(false);
		SpawnActor->DisableComponentsSimulatePhysics();
		Cast<UStaticMeshComponent>(SpawnActor->GetComponentByClass(UStaticMeshComponent::StaticClass()))->SetCollisionProfileName(TEXT("OverlapAll"));
		SpawnActor->SetActorLabel(DuplicatedName.ToString() + "copy");
		ClonedArticulatedObjects.Add(SpawnActor);
		//UE_LOG(LogTemp, Warning, TEXT("DuplicateArticulatedMesh %s"), *ClonedArticulatedObjects[Index]->GetName());

		// Do this for the Handler attach to the drawer,change the attach to location
		if (ArticulatedObjects[Index]->GetAttachParentActor() != NULL)
		{
			
			SpawnActor->AttachToActor(RootActor, FAttachmentTransformRules::KeepWorldTransform);
		}
	}

}

// check overlapping begin for controller enters in the frustum
void AMyPawn::OnOverlapBeginBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->GetName().Contains("WSGBase"))
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
		if (OtherActor->GetName().Contains("WSGBase"))
		{
			OtherComp->SetRenderCustomDepth(true);
		}
	}
}





