// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Public/TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SpawnActor.h"
#include "RobotView.generated.h"

UCLASS()
class GRIPPERGRASP_API ARobotView : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARobotView();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// MC Root
	USceneComponent * MCRoot;

	// MC Root
	USceneComponent * CameraRoot;

	// Left Motion Controller
	UPROPERTY(EditAnywhere, Category = "MC")
		UMotionControllerComponent* MCLeft;

	// Right Motion Controller
	UPROPERTY(EditAnywhere, Category = "MC")
		UMotionControllerComponent* MCRight;

	// Display MC controller mesh
	UPROPERTY(EditAnywhere, Category = "MC", DisplayName = "Visualize MC Meshes")
		bool bVisualizeMCMeshes;


	// VR Camera
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MC")
	UCameraComponent* VRCamera;

	// Trigger sphere 
	UPROPERTY(EditAnywhere, Category = "TriggerSphere")
		USphereComponent* LeftSphere;

	// Visual cues for Left Sphere
	UPROPERTY(EditAnywhere, Category = "TriggerSphere")
		UStaticMeshComponent* LeftSphereVC;

	// Visual cues for Right Sphere
	UPROPERTY(EditAnywhere, Category = "TriggerSphere")
		UStaticMeshComponent* RightSphereVC;

	// Frustum Component
	UPROPERTY(EditAnywhere, Category = "Frustum")
		UStaticMeshComponent* Frustum;

	// Spring Arm component for the capsule trigger to attach to
	UPROPERTY(EditAnywhere, Category = "Capsule")
		USpringArmComponent* SpringArmComponent;

	// Capsule Component for body collision detection
	UPROPERTY(EditAnywhere, Category = "Capsule")
		UCapsuleComponent* CapsuleTrigger;

	// Visual Cue for capsule triiger component
	UPROPERTY(EditAnywhere, Category = "Capsule")
		UStaticMeshComponent* CapsuleTriggerVC;

	// Count the number when a static actor is overlapping with capsule collider
	UPROPERTY(EditAnywhere, Category = "Capsule")
		int OverlapNum;

	// The bool desides whether the camera is outside of range or not 
	UPROPERTY(EditAnywhere, Category = "Capsule")
		bool bCameraOutside;


	// Declare overlap begin function for capsule trigger
	UFUNCTION()
		void OnCapsuleOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Declare overlap end function for capsule trigger
	UFUNCTION()
		void OnCapsuleOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Left GripperBase
	UPROPERTY(EditAnywhere, Category = "Gripper")
		UStaticMeshComponent* GBLeft;


	// Right GripperBase
	UPROPERTY(EditAnywhere, Category = "Gripper")
		UStaticMeshComponent* GBRight;

	UPROPERTY(EditAnywhere)
		UHapticFeedbackEffect_Base* mHapticFeedback;

	// Declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void PlayHapticFeedback();

	// Left trigger pressed
	UFUNCTION()
		virtual void MotionControlLeftTriggerPressed();

	UPROPERTY(EditAnywhere)
		TSubclassOf<ASpawnActor> ActorToSpawn;

	UPROPERTY(EditAnywhere)
		UForceFeedbackEffect* ForceFeedbackEffect;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> UpdateMeshes;

	UPROPERTY(EditAnywhere)
		TArray<AStaticMeshActor*> ClonedObjects;

	// Function for camera behavior when the camera is outside the range
	UFUNCTION()
		void CameraRangeDetection();


private:
	
	UPROPERTY(EditAnywhere, Category = "HMD")
	float NearClippingPlane;

	UPROPERTY(EditAnywhere, Category = "HMD")
	float FarClippingPlane;

	UPROPERTY(EditAnywhere, Category = "HMD")
	float bControllerVibrate;

	FTimerHandle TimerHandle;

	// Material for capsule trigger
	UPROPERTY(EditAnywhere, Category = "Capsule")
	UMaterial* CapsuleMaterial;


	

	
	
};
