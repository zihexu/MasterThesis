// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/DrawFrustumComponent.h"
#include "MyCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class GRIPPERGRASP_API UMyCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UMyCameraComponent();


	UFUNCTION()
		virtual void RefreshVisualRepresentation() override;



	
	
	
};
