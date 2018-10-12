// Fill out your copyright notice in the Description page of Project Settings.

#include "NewCameraComponent.h"
#include "Components/DrawFrustumComponent.h"

UNewCameraComponent::UNewCameraComponent()
{



	CameraFrustum = CreateDefaultSubobject<UDrawFrustumComponent>(TEXT("CameraFrustum"));
	CameraFrustum->SetupAttachment(this);

	//VRCamera->DrawFrustum = CameraFrustum;

}


