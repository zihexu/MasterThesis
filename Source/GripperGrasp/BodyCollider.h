// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Public/Containers/Array.h"
#include "MCPawn.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BodyCollider.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIPPERGRASP_API UBodyCollider : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBodyCollider();



	UPROPERTY(EditAnywhere, Category = ViveTracker)
		UMotionControllerComponent* ViveTracker2;

	UPROPERTY(EditAnywhere, Category = ViveTracker)
		float OffsetValue;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	// Root Robot Actor
	UPROPERTY(EditAnywhere, Category = "TriggerArea")
		AStaticMeshActor* RootForRobotBase;

	// Body Collider Actor
	UPROPERTY(EditAnywhere, Category = "TriggerArea")
		AStaticMeshActor* BodyCollider;

	// Body Collider Actor
	UPROPERTY(EditAnywhere, Category = "TriggerSpArea")
		AStaticMeshActor* VisualCueCollider;

	// Declare overlap begin function
	UFUNCTION()
		void OnOverlapBeginBase(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	// Declare overlap begin function
	UFUNCTION()
		void OnOverlapBeginVisualCueCollider(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Declare overlap end function
	UFUNCTION()
		void OnOverlapEndVisualCueCollider(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Spawn ArrowIndicator class
	UPROPERTY(EditAnywhere, Category = ArrowIndicator)
		TSubclassOf<class ASpawnActor> SpawnArrowIndicator;

	// Spawn ArrowIndicator class
	UPROPERTY(EditAnywhere, Category = ArrowIndicator)
		bool bShowIndicator;

	UPROPERTY()
		TArray<AActor*> OverlappingActors;

	UPROPERTY()
		ASpawnActor* ArrowIndicator;

	UPROPERTY()
		TArray<FVector> HitPosition;

	UPROPERTY()
		int i = -1;

	UPROPERTY(EditAnywhere, Category = ArrowIndicator)
		bool bEnableGameRule;

	UPROPERTY()
		int OverlapNum;


	UPROPERTY()
		UCameraComponent* MyCamera;

	/** Sound to play each time collide event happens */
	UPROPERTY(EditAnywhere)
		class USoundBase* CollideSound;
	
	// Spawn Arrow Indicator at begin play
	UFUNCTION()
		void SpawnArrowIndicatorFunction();

};
