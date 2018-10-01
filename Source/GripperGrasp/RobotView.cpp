// Fill out your copyright notice in the Description page of Project Settings.



#include "RobotView.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h"
#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"



// Sets default values
ARobotView::ARobotView()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Possess player automatically
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bVisualizeMCMeshes = true;
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

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(VRCamera);
	//TriggerBox->bGenerateOverlapEvents = true;
	//Register Events
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARobotView::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARobotView::OnOverlapEnd);

}

// Called when the game starts or when spawned
void ARobotView::BeginPlay()
{
	Super::BeginPlay();



	IHeadMountedDisplay* HMD = GEngine->XRSystem.IsValid() ? GEngine->XRSystem->GetHMDDevice() : nullptr;

	if (HMD)
	{
		HMD->EnableHMD(true);
		//UE_LOG(LogTemp, Warning, TEXT("found HMD here"));
		HMD->SetClippingPlanes(NearClippingPlane, FarClippingPlane);
	}

	
}

// Called every frame
void ARobotView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ARobotView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARobotView::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	
	// check if Actors do not equal nullptr and that 
	if (OtherActor&&OtherComp) {
		UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
		//printFString("Overlapped Actor = %s", *OverlappedComp->GetName());
	}
}

void ARobotView::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor&&OtherComp ) {

		//printFString("%s has left the Trigger Box", *OtherActor->GetName());
	}
}





