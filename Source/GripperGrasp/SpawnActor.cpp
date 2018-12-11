// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnActor.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"

// Sets default values
ASpawnActor::ASpawnActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = Root;
	Root->SetMobility(EComponentMobility::Movable);
	this->SetActorHiddenInGame(true);

	SpawnComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnComponent"));
	SpawnComponent->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SpawnComponentAsset(TEXT("/Game/Models/Player/ArrowIndicator/ArrowIndicator.ArrowIndicator"));
	if (SpawnComponentAsset.Succeeded())
	{
		SpawnComponent->SetStaticMesh(SpawnComponentAsset.Object);
	}

	
}

// Called when the game starts or when spawned
void ASpawnActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

