// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMeshActor.h"
#include "SpawnActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MyPawn.generated.h"


///** Notify when an object is grasped */
//DECLARE_MULTICAST_DELEGATE_OneParam(FUpdatePerception, float /*Time*/);


UCLASS()
class GRIPPERGRASP_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
		AStaticMeshActor* RootActor;

	UPROPERTY(EditAnywhere)
		TMap < AStaticMeshActor*, AStaticMeshActor* > RealToVisual;

	UPROPERTY(EditAnywhere)
		TMap < AStaticMeshActor*, AStaticMeshActor* > VisualToReal;
	
	//// Event called when update perception
	//FUpdatePerception OnUpdatePerception;
	

private:

	// VR Camera
	UPROPERTY(EditAnywhere)
	UCameraComponent* VRCamera;

	// Motion controller root component (camera and motion controllers will be relative to this)
	UPROPERTY(EditAnywhere, Category = "MC")
		USceneComponent* MCRoot;

	UPROPERTY(EditAnywhere)
		AActor* Frustum;

	UPROPERTY(EditAnywhere)
		AStaticMeshActor* SMFrustum;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> UpdateMeshes;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> AllVisualObjects;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> DynamicActors;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> DynamicStaticMeshActors;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> ClonedDynamicObjects;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> ArticulatedActors;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> ArticulatedStaticMeshActors;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> ClonedArticulatedObjects;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> CurrentVisibleMeshes;

	UPROPERTY(EditAnywhere)
		FVector ContactPointLoc;

	UFUNCTION()
		void Start();


	//Update View Function
	UFUNCTION()
		void UpdateView();

	UFUNCTION()
		void SpawnDynamicObjects();

	UFUNCTION()
		void SpawnArticulatedObjects();




	// Declare overlap begin function 
	UFUNCTION()
		void OnOverlapBeginBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Declare overlap end function
	UFUNCTION()
		void OnOverlapEndBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
