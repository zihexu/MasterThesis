// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "MyGripperPawn.generated.h"

UCLASS()
class GRIPPERGRASP_API AMyGripperPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyGripperPawn();

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
	USceneComponent* MCRoot;


	// Left Motion Controller
	UMotionControllerComponent* MCLeft;

	// Right Motion Controller
	UMotionControllerComponent* MCRight;


	// Right GripperBase
	UPROPERTY(EditAnywhere, Category = "Gripper")
		UStaticMeshComponent* GBRight;

	// Right GripperFinger_Left
	UPROPERTY(EditAnywhere, Category = "Gripper")
		UStaticMeshComponent* GFRightl;

	// Right GripperFinger_Right
	UPROPERTY(EditAnywhere, Category = "Gripper")
		UStaticMeshComponent* GFRightr;

    // Output variable
	float Force;

	// GripperFingerLeft original relative location
	FVector GFRightl_ORL;

	// GripperFingerRight original relative location
	FVector GFRightr_ORL;



	// Gripper Grasp Event
	void GripperGrasp(float value);

	
	UFUNCTION()
	virtual void MotionControlLeftTriggerPressed();

	UFUNCTION()
	virtual void MotionControlLeftTriggerReleased();
	
	

	UFUNCTION()
	virtual void MotionControlRightTriggerPressed();

	UFUNCTION()
	virtual void MotionControlRightTriggerReleased();

	


	
};
