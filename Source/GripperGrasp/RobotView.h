// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "HeadMountedDisplay/Public/MotionControllerComponent.h"
#include "Components/BoxComponent.h"
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

	// VR Camera
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MC")
	UCameraComponent* VRCamera;

	UPROPERTY(EditAnywhere, Category = "TriggerBox")
		UBoxComponent* TriggerBox;

	// declare overlap begin function
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
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

	UPROPERTY(EditAnywhere, Category = "HMD")
	float NearClippingPlane;

	UPROPERTY(EditAnywhere, Category = "HMD")
	float FarClippingPlane;



	

	
	
};
