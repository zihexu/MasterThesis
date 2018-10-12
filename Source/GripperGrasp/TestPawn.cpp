// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPawn.h"
#include "XRMotionControllerBase.h"


// Sets default values
ATestPawn::ATestPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Possess player automatically
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//Create Root Component
	MCRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MCRoot;

	// Create camera component
	VRCamera = CreateDefaultSubobject<UMyCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(MCRoot);


}

// Called when the game starts or when spawned
void ATestPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


