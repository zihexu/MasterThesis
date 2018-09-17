// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawn.h"
#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"
#include "XRMotionControllerBase.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Possess player automatically
	AutoPossessPlayer = EAutoReceiveInput::Player0;


	// Crate MC root
	MCRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MCRoot"));
	//MCRoot->SetupAttachment(GetRootComponent());
	RootComponent = MCRoot;

	// Create camera component
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(MCRoot);

	// Create the right motion controller
	MCLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCLeft"));
	MCLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MCLeft->SetupAttachment(MCRoot);

	// Create the left motion controller
	MCRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCRight"));
	MCRight->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	MCRight->SetupAttachment(MCRoot);

	// Create the left GripperBase
	GBLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripperBaseLeft"));
	GBLeft->SetupAttachment(MCLeft);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GBLeftAsset(TEXT("/Game/StarterContent/Models/Gripper/GripperBase/SM_GripperBase.SM_GripperBase"));
	if (GBLeftAsset.Succeeded())
	{
		GBLeft->SetStaticMesh(GBLeftAsset.Object);
		GBLeft->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	// Create the right GripperBase
	GBRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripperBaseRight"));
	GBRight->SetupAttachment(MCRight);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> GBRightAsset(TEXT("/Game/StarterContent/Models/Gripper/GripperBase/SM_GripperBase.SM_GripperBase"));
	if (GBRightAsset.Succeeded())
	{
		GBRight->SetStaticMesh(GBRightAsset.Object);
		GBRight->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

