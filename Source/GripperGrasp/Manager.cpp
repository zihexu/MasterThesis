// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager.h"
#include "Public/EngineUtils.h"
#include "Engine/StaticMeshActor.h"



// Sets default values for this component's properties
UManager::UManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	
	// ...
}



// Called when the game starts
void UManager::BeginPlay()
{
	Super::BeginPlay();

	//Iterate all actors and disable the visability of the moving objects
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AStaticMeshActor *Mesh = *ActorItr;
		if (Mesh->IsRootComponentMovable())
		{
			if(Mesh->ActorHasTag(TEXT("MovableObjects")))
			Mesh->SetActorHiddenInGame(true);

			//ActorItr->GetStaticMeshComponent()->SetHiddenInGame(true);

		}

	}
	// ...
	
}


// Called every frame
void UManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


