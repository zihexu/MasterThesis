// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCollisionComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Classes/Engine/Scene.h"



// Sets default values for this component's properties
UMyCollisionComponent::UMyCollisionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
    

	

	// ...
}


// Called when the game starts
void UMyCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Set default state for LeftConreoller Collision sphere
	if (UStaticMeshComponent* StaticMeshComp = LeftSphereVC->GetStaticMeshComponent())
	{
		
		StaticMeshComp->SetMobility(EComponentMobility::Movable);
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetEnableGravity(false);
		StaticMeshComp->SetCollisionProfileName(TEXT("Trigger"));

		//Register Events for LeftSphereVC

		LeftSphereVC->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &UMyCollisionComponent::OnOverlapBeginLeft);
		LeftSphereVC->GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &UMyCollisionComponent::OnOverlapEndLeft);


	}
	// Set default state for RightConreoller Collision sphere
	if (UStaticMeshComponent* StaticMeshComp = RightSphereVC->GetStaticMeshComponent())
	{
		StaticMeshComp->SetMobility(EComponentMobility::Movable);
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetEnableGravity(false);
		StaticMeshComp->SetCollisionProfileName(TEXT("Trigger"));
	}




	// Set default state for LeftSphereSmall
	if (UStaticMeshComponent* StaticMeshComp = LeftSphereSmall->GetStaticMeshComponent())
	{

		StaticMeshComp->SetMobility(EComponentMobility::Movable);
		StaticMeshComp->SetSimulatePhysics(false);
		StaticMeshComp->SetEnableGravity(false);
		StaticMeshComp->SetCollisionProfileName(TEXT("Trigger"));

		//Register Events for LeftSphereVC

		LeftSphereSmall->GetStaticMeshComponent()->OnComponentBeginOverlap.AddDynamic(this, &UMyCollisionComponent::OnOverlapBeginLeftSmall);
		LeftSphereSmall->GetStaticMeshComponent()->OnComponentEndOverlap.AddDynamic(this, &UMyCollisionComponent::OnOverlapEndLeftSmall);


	}

	// ...
	
}


// Called every frame
void UMyCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// Teleport LeftSphereVC
	if (UStaticMeshComponent* StaticMeshComp = LeftSphereVC->GetStaticMeshComponent())
	{
		StaticMeshComp->SetWorldLocation(GetComponentLocation()+FVector(20.0f,0.0f,0.0f),
			false, (FHitResult*)nullptr, ETeleportType::None);
		if (bLeftSphereVC)
		{
			LeftSphereVC->SetActorHiddenInGame(false);
			
		}
		else if (!bLeftSphereVC)
		{
			LeftSphereVC->SetActorHiddenInGame(true);
		}
	}
	// Teleport RightSphereVC
	if (UStaticMeshComponent* StaticMeshComp = RightSphereVC->GetStaticMeshComponent())
	{
		StaticMeshComp->SetWorldLocation(GetComponentLocation()+FVector(-20.0f, 0.0f, 0.0f),
			false, (FHitResult*)nullptr, ETeleportType::None);
	}
	
	// Teleport RightSphereSmall
	if (UStaticMeshComponent* StaticMeshComp = LeftSphereSmall->GetStaticMeshComponent())
	{
		StaticMeshComp->SetWorldLocation(GetComponentLocation() + FVector(20.0f, 0.0f, 0.0f),
			false, (FHitResult*)nullptr, ETeleportType::None);
	}
	
	// ...
}

void UMyCollisionComponent::OnOverlapBeginLeft(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor!=NULL)
	{
		if (OtherActor->GetName().Contains(FString("WSGBaseLeft")))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor"));
			OtherComp->SetLinearDamping(0.01f);
			OtherComp->SetAngularDamping(0.0f);
			OtherComp->SetRenderCustomDepth(false);
		}
	}
	
}

void UMyCollisionComponent::OnOverlapEndLeft(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (OtherActor->GetName().Contains(FString("WSGBaseLeft")))
		{
			OtherComp->SetLinearDamping(50.0f);
			OtherComp->SetAngularDamping(50.0f);
			OtherComp->SetRenderCustomDepth(true);
			//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor = %s"), *OtherActor->GetName());
		}
	}
}



void UMyCollisionComponent::OnOverlapBeginLeftSmall(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != NULL)
	{
		if (OtherActor->GetName().Contains(FString("WSGBaseLeft")))
		{
			bLeftSphereVC = false;
			
		}
	}
}

void UMyCollisionComponent::OnOverlapEndLeftSmall(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != NULL)
	{
		if (OtherActor->GetName().Contains(FString("WSGBaseLeft")))
		{
			bLeftSphereVC = true;
			//UE_LOG(LogTemp, Warning, TEXT("shouldshowcollision sphere"));
		}
	}
}



