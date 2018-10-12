// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "NewCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class GRIPPERGRASP_API UNewCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

		UNewCameraComponent();

	// MC Root
	USceneComponent * MCRoot;

	UPROPERTY(EditAnywhere)
		UDrawFrustumComponent* CameraFrustum;
	
	
};
