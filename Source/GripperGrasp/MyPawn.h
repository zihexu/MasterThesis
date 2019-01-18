// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMeshActor.h"
#include "SpawnActor.h"
#include "MyPawn.generated.h"

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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	

private:

	// VR Camera
	UCameraComponent* VRCamera;

	// Motion controller root component (camera and motion controllers will be relative to this)
	UPROPERTY(EditAnywhere, Category = "MC")
		USceneComponent* MCRoot;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> UpdateMeshes;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> ClonedObjects;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASpawnActor> ActorToSpawn;

	//Update View Function
	UFUNCTION()
		void UpdateView();
	
};
