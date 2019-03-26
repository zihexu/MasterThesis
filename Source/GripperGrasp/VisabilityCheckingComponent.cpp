// Fill out your copyright notice in the Description page of Project Settings.

#include "VisabilityCheckingComponent.h"

// Sets default values for this component's properties
UVisabilityCheckingComponent::UVisabilityCheckingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVisabilityCheckingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	
}

// Called every frame
void UVisabilityCheckingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Check if this actor is recently rendered
	//UE_LOG(LogTemp, Warning, TEXT("script running"));
	
	if (Lefthand != NULL)
	{
		UStaticMeshComponent* LH = Lefthand->GetStaticMeshComponent();
		if (Lefthand->WasRecentlyRendered(0.001f) != true)
		{

			LH->SetRenderCustomDepth(true);
			UE_LOG(LogTemp, Warning, TEXT("LeftHand is outside"));
		}
		else
		{
			LH->SetRenderCustomDepth(false);
			UE_LOG(LogTemp, Warning, TEXT("LeftHand is inside"));
		}
	}

	if (Righthand != NULL)
	{
		UStaticMeshComponent* RH = Righthand->GetStaticMeshComponent();
	
		if (Righthand->WasRecentlyRendered(0.001f) != true)
		{

			RH->SetRenderCustomDepth(true);
			UE_LOG(LogTemp, Warning, TEXT("LeftHand is outside"));
		}
		else
		{
			RH->SetRenderCustomDepth(false);
		}
	}

	



}

