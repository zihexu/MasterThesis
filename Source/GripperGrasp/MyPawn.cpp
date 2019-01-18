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

}

