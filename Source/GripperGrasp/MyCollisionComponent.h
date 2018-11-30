// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Components/SceneComponent.h"
#include "MyCollisionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIPPERGRASP_API UMyCollisionComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyCollisionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Visual cues for Left Sphere
	UPROPERTY(EditAnywhere, Category = "TriggerSphere")
		AStaticMeshActor* LeftSphereVC;

	// Visual cues for Right Sphere
	UPROPERTY(EditAnywhere, Category = "TriggerSphere")
		AStaticMeshActor* RightSphereVC;

	// Visual Cue for capsule triiger component
	UPROPERTY(EditAnywhere, Category = "Capsule")
		AStaticMeshActor* BodyCollision;



	// Count the number when a static actor is overlapping with capsule collider
	UPROPERTY(EditAnywhere, Category = "Capsule")
		int OverlapNum;

	// The bool desides whether the camera is outside of range or not 
	UPROPERTY(EditAnywhere, Category = "Capsule")
		bool bCameraOutside;
		
	// Declare overlap begin function
	UFUNCTION()
		void OnOverlapBeginLeft(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Declare overlap end function
	UFUNCTION()
		void OnOverlapEndLeft(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Declare overlap begin function
	UFUNCTION()
		void OnOverlapBeginBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Declare overlap end function
	UFUNCTION()
		void OnOverlapEndBody(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



};
