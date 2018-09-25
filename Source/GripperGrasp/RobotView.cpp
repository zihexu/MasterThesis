// Fill out your copyright notice in the Description page of Project Settings.

#include "RobotView.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h"
#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"


// Sets default values
ARobotView::ARobotView()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Possess player automatically
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create Root Component
	MCRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MCRoot;

	// Create camera component
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	
	VRCamera->SetupAttachment(MCRoot);


	// Create the left motion controller
	MCLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCLeft"));
	MCLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MCLeft->SetupAttachment(MCRoot);

	// Create the right motion controller
	MCRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCRight"));
	MCRight->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	MCRight->SetupAttachment(MCRoot);
}

// Called when the game starts or when spawned
void ARobotView::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARobotView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetClippingPlanes(1.0, 100.0);
}

// Called to bind functionality to input
void ARobotView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



