// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "MyPawn.generated.h"

UCLASS()
class MYPROJECT_API AMyPawn : public APawn
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

	// VR Camera
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MC")
		UCameraComponent* VRCamera;
	

private:
	// MC Root
	USceneComponent * MCRoot;

	// Left Motion Controller
	UMotionControllerComponent* MCLeft;

	// Right Motion Controller
	UMotionControllerComponent* MCRight;


	UPROPERTY(EditAnywhere, Category = "Gripper")
	UStaticMeshComponent* GBLeft;

	UPROPERTY(EditAnywhere, Category = "Gripper")
	UStaticMeshComponent* GBRight;
};

