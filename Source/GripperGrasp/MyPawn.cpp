// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "Engine/World.h"
#include "Public/EngineUtils.h"
#include "GameFramework/PlayerController.h"


// Sets default values
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

	// Find all movable objects
	UWorld* const World = GetWorld();
	for (TActorIterator<AStaticMeshActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsRootComponentMovable())
			if (ActorItr->ActorHasTag("MovableObjects"))
			{
				AStaticMeshActor* Mesh = *ActorItr;
				UE_LOG(LogTemp, Warning, TEXT("find a actor %s"), *Mesh->GetName());
				UpdateMeshes.Add(Mesh);
			
			}
	}

	// Spawn the template actors
	for (int32 Index = 0; Index != UpdateMeshes.Num(); ++Index)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AStaticMeshActor* DuplicateMesh = UpdateMeshes[Index];
		SpawnParams.Template = DuplicateMesh;

		// The random error location to spawn  + FMath::VRand() * 5
		FVector WorldLocation = DuplicateMesh->GetActorLocation();

		FVector SpawnLocation = FMath::VRand() * 1;

		// The rotation to spawn
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		// The name of the spawned actor
		FName DuplicatedName = DuplicateMesh->GetFName();

		// Spawn the objects 
		AStaticMeshActor* SpawnActor = World->SpawnActor<AStaticMeshActor>(DuplicateMesh->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
		SpawnActor->SetActorLocation(WorldLocation+ FMath::VRand() * 1);
		SpawnActor->SetActorRotation(DuplicateMesh->GetActorRotation());
		SpawnActor->SetActorHiddenInGame(false);
		SpawnActor->DisableComponentsSimulatePhysics();
		SpawnActor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("OverlapAll"));
		SpawnActor->SetActorLabel(UpdateMeshes[Index]->GetName()+"copy");
		ClonedObjects.Add(SpawnActor);

		//Attach the newly spawned handle, Todo: should attach to the newly spawned parent objects
		if (DuplicateMesh->GetAttachParentActor() != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("DuplicateMesh %s"), *DuplicateMesh->GetName());
			SpawnActor->AttachToActor(RootActor, FAttachmentTransformRules::KeepWorldTransform);
		}
		//UE_LOG(LogTemp, Warning, TEXT("location %s"), *SpawnActor->GetName());
		//UE_LOG(LogTemp, Warning, TEXT("spawn location %s"), *SpawnActor->GetActorLocation().ToString());
		
		
	}
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("UpdateViewTouchpad", EInputEvent::IE_Pressed, this, &AMyPawn::UpdateView);

}

void AMyPawn::UpdateView()
{
	UE_LOG(LogTemp, Warning, TEXT("Update button pressed"));
	//Once UpdateView Button clicked, update the location of all cloned objects
	for (int32 Index = 0; Index != ClonedObjects.Num(); ++Index)
	{
		AStaticMeshActor* Reference = UpdateMeshes[Index];
		FVector NewLocation = Reference->GetActorLocation()+ FMath::VRand() * 1;
		//Teleport the cloned objects
		ClonedObjects[Index]->SetActorLocation(NewLocation, false, (FHitResult*)nullptr, ETeleportType::None);
		//UE_LOG(LogTemp, Warning, TEXT("destroyed actor %s"), *ClonedObjects[Index]->GetName());
		ClonedObjects[Index]->SetActorRotation(Reference->GetActorRotation());
		//
	}


	/*
	
	// Destroy cloned objects
	for (int32 Index = 0; Index != ClonedObjects.Num(); ++Index)
	{

		ClonedObjects[Index]->Destroy();
		//UE_LOG(LogTemp, Warning, TEXT("destroyed actor %s"), *ClonedObjects[Index]->GetName());

	}
	ClonedObjects.Empty();

	// Find all movable objects
	UWorld* const World = GetWorld();
	for (TActorIterator<AStaticMeshActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsRootComponentMovable())
			if(ActorItr->ActorHasTag("MovableObjects"))
			{
			AStaticMeshActor* Mesh = *ActorItr;
			//UE_LOG(LogTemp, Warning, TEXT("find a actor %s"), *Mesh->GetName());
			UpdateMeshes.Add(Mesh);
			}
	}
	// Spawn the template actors
	for (int32 Index = 0; Index != UpdateMeshes.Num(); ++Index)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AStaticMeshActor* DuplicateMesh = UpdateMeshes[Index];
		SpawnParams.Template = DuplicateMesh;

		// The random error location to spawn  + FMath::VRand() * 5
		FVector SpawnLocation = FMath::VRand() * 1;

		// The rotation to spawn
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

		// Spawn the objects 
		AStaticMeshActor* SpawnActor = World->SpawnActor<AStaticMeshActor>(DuplicateMesh->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
		SpawnActor->SetActorHiddenInGame(false);
		SpawnActor->DisableComponentsSimulatePhysics();
		SpawnActor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("OverlapAll"));
		ClonedObjects.Add(SpawnActor);
		//UE_LOG(LogTemp, Warning, TEXT("location %s"), *DuplicateMesh->GetActorLocation().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("spawn location %s"), *SpawnActor->GetActorLocation().ToString());
	}
	// Empty all elements in array
	UpdateMeshes.Empty();
	*/
}




