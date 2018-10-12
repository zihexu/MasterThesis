// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UObjectGlobals.h"
#include "EngineGlobals.h"

UMyCameraComponent::UMyCameraComponent()
{
	




}

void UMyCameraComponent::RefreshVisualRepresentation()
{
	Super::RefreshVisualRepresentation();
	const float FrustumDrawDistance = 100.0f;
}
