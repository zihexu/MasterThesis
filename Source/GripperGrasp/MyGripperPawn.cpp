// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGripperPawn.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h"
#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"



// Sets default values
AMyGripperPawn::AMyGripperPawn()
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

	
	//Create Left GripperBase Component
	GBLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripperBaseLeft"));
	GBLeft->SetupAttachment(MCLeft);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GBLeftAsset(TEXT("/Game/Models/Gripper/GripperBase/SM_GripperBase.SM_GripperBase"));
	if (GBLeftAsset.Succeeded())
	{
		GBLeft->SetStaticMesh(GBLeftAsset.Object);
		GBLeft->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	//Create Right GripperBase Component
	GBRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripperBaseRight"));
	GBRight->SetupAttachment(MCRight);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GBRightAsset(TEXT("/Game/Models/Gripper/GripperBase/SM_GripperBase.SM_GripperBase"));
	if (GBRightAsset.Succeeded())
	{
		GBRight->SetStaticMesh(GBRightAsset.Object);
		//GBRight->SetRelativeLocation(FVector(90.0f, 0.0f, 0.0f));
		//GBRight->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		GBRight->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	}

	//Create Right GripperFingerl Component
	GFRightl = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripperFingerRightl"));
	GFRightl->SetupAttachment(MCRight);
	GFRightl->SetSimulatePhysics(true);
	
	GFRightl->BodyInstance.bLockXRotation = true;
	GFRightl->BodyInstance.bLockYRotation = true;
	GFRightl->BodyInstance.bLockZRotation = true;
	
	
	GFRightl->SetEnableGravity(false);
	GFRightl->SetCollisionProfileName(TEXT("BlockAll"));
	GFRightl_ORL = FVector(0.0f, 7.0f, -5.2f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GFRightlAsset(TEXT("/Game/Models/Gripper/GripperFinger/GripperFingerLeft/SM_GripperFingerLeft.SM_GripperFingerLeft"));
	if (GFRightlAsset.Succeeded())
	{
		GFRightl->SetStaticMesh(GFRightlAsset.Object);
		GFRightl->SetRelativeLocation(GFRightl_ORL);
		GFRightl->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));

	}

	//Create Right GripperFingerr Component
	GFRightr = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GripperFingerRightr"));
	GFRightr->SetupAttachment(MCRight);
	GFRightr->SetSimulatePhysics(true);
	GFRightr->BodyInstance.bLockXRotation = true;
	GFRightr->BodyInstance.bLockYRotation = true;
	GFRightr->BodyInstance.bLockZRotation = true;
	GFRightr->SetEnableGravity(false);
	GFRightr->SetCollisionProfileName(TEXT("BlockAll"));
	GFRightr_ORL = FVector(0.0f, 7.0f, 7.2f);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>GFRightrAsset(TEXT("/Game/Models/Gripper/GripperFinger/GripperFingerRight/SM_GripperFingerRight.SM_GripperFingerRight"));
	if (GFRightrAsset.Succeeded())
	{
		GFRightr->SetStaticMesh(GFRightlAsset.Object);
		GFRightr->SetRelativeLocation(GFRightr_ORL);
		GFRightr->SetRelativeRotation(FRotator(270.0f, 0.0f, 0.0f));
	}
   
}

// Called when the game starts or when spawned
void AMyGripperPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyGripperPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//UE_LOG(LogTemp, Warning, TEXT("Add force to GripperFingerLeft is %f"),Force );

	
	// Set GripperFingerLeft back to position when no force
	if (Force == 0)
	{
		if (GFRightl->GetRelativeTransform().GetLocation().Z > -5.2f)
		{
			GFRightl->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, -5.0f));
		}
		if (GFRightl->GetRelativeTransform().GetLocation().Z <= -5.2f)
		{
			GFRightl->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		}

	}

	// Set GripperFingerRight back to position when no force
	if (Force == 0)
	{
		if (GFRightr->GetRelativeTransform().GetLocation().Z < 7.2f)
		{
			GFRightr->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 5.0f));
		}
		if (GFRightr->GetRelativeTransform().GetLocation().Z >= 7.2f)
		{
			GFRightr->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 0.0f));
		}

	}
	
/*
	// Set GripperFinger back to position when no force
	if (Force == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Add force to GripperFingerLeft is %f"), Force);

		GFRightl->SetupAttachment(GBRight);
		GFRightl->SetRelativeLocation(GFRightl_ORL);
		GFRightr->SetupAttachment(GBRight);
		GFRightr->SetRelativeLocation(GFRightr_ORL);

	}
	*/

}

// Called to bind functionality to input
void AMyGripperPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//Motion controller bindings
	PlayerInputComponent->BindAxis("RightGrasp", this, &AMyGripperPawn::GripperGrasp);
	InputComponent->BindAction("LeftTriggerPressed", EInputEvent::IE_Pressed, this, &AMyGripperPawn::MotionControlLeftTriggerPressed);
	InputComponent->BindAction("LeftTriggerPressed", EInputEvent::IE_Released, this, &AMyGripperPawn::MotionControlLeftTriggerReleased);
	InputComponent->BindAction("RightTriggerPressed", EInputEvent::IE_Pressed, this, &AMyGripperPawn::MotionControlRightTriggerPressed);
	InputComponent->BindAction("RightTriggerPressed", EInputEvent::IE_Released, this, &AMyGripperPawn::MotionControlRightTriggerReleased);

}
 
// Add force to GripperFinger
void AMyGripperPawn::GripperGrasp(float value)
{

	GFRightl->AddForce(FVector(0.0f, 0.0f, 5.0f*value));
	GFRightr->AddForce(FVector(0.0f, 0.0f, -5.0f*value));
	Force = 5.0f*value;

	

}

void AMyGripperPawn::MotionControlLeftTriggerPressed()
{
}

void AMyGripperPawn::MotionControlLeftTriggerReleased()
{
}

void AMyGripperPawn::MotionControlRightTriggerPressed()
{
}

void AMyGripperPawn::MotionControlRightTriggerReleased()
{
	/*
	UE_LOG(LogTemp, Warning, TEXT("Add force to GripperFingerLeft is %f"), Force);
	if (GFRightl->GetRelativeTransform().GetLocation().X > -5.2f )
	{
		GFRightl->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, 5.0f));
		
	}
	*/
	
	
}



