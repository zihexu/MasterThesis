// Fill out your copyright notice in the Description page of Project Settings.



#include "RobotView.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "XRMotionControllerBase.h"
#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "InputCoreTypes.h"
#include "Public/EngineUtils.h"
#include "Classes/Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Engine/World.h"





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

	//Create Camera RootComponent
	CameraRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CameraRootComponent"));
	CameraRoot->SetupAttachment(MCRoot);


	// Create camera component
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(CameraRoot);

	// Create Frustum Component
	Frustum = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Frustum"));
	Frustum->SetupAttachment(VRCamera);
	Frustum->SetCollisionProfileName(TEXT("No Collision"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>FrustumAsset(TEXT("/Game/Models/Frustum/SM_Frustum.SM_Frustum"));
	if (FrustumAsset.Succeeded())
	{
		Frustum->SetStaticMesh(FrustumAsset.Object);
		Frustum->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Frustum->SetRelativeLocation(FVector(97.0f, 0.0f, 0.0f));
		Frustum->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}

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
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	//TriggerBox->bGenerateOverlapEvents = true;
	//Register Events
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ARobotView::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ARobotView::OnOverlapEnd);

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
	/*
	ConstructorHelpers::FObjectFinder<UHapticFeedbackEffect_Base> hapticFeedbackFinder(TEXT("/Game/HapticFeedback/HapticFeedback.HapticFeedback"));
	if(hapticFeedbackFinder.Succeeded())
		UE_LOG(LogTemp, Warning, TEXT("find object"));
	mHapticFeedback = hapticFeedbackFinder.Object;
	*/

}

// Called when the game starts or when spawned
void ARobotView::BeginPlay()
{
	Super::BeginPlay();

	//Iterate all actors and disable the visability of the moving objects
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		//AStaticMeshActor *Mesh = *ActorItr;
		if (ActorItr->IsRootComponentMovable())
		{
			ActorItr->SetActorHiddenInGame(false);
			ActorItr->GetStaticMeshComponent()->SetHiddenInGame(false);
			
		}
			
	}


/*
	IHeadMountedDisplay* HMD = GEngine->XRSystem.IsValid() ? GEngine->XRSystem->GetHMDDevice() : nullptr;
	if (HMD)
	{
		HMD->EnableHMD(true);
		//UE_LOG(LogTemp, Warning, TEXT("found HMD here"));
		HMD->SetClippingPlanes(NearClippingPlane, FarClippingPlane);
	}
*/


	
}

// Called every frame
void ARobotView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bControllerVibrate)
	{
		//UE_LOG(LogTemp, Warning, TEXT("start vibrate"));
		
	}
	TriggerBox->SetRenderCustomDepth(true);
	APlayerController * MyPc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPc)
	{
		//MyPc->ClientPlayForceFeedback(ForceFeedbackEffect, false, false, NAME_None);
		PlayHapticFeedback();
	}

	
}

// Called to bind functionality to input
void ARobotView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Motion Controller input binding
	InputComponent->BindAction("LeftTriggerPressed", EInputEvent::IE_Pressed, this, &ARobotView::MotionControlLeftTriggerPressed);

}

void ARobotView::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherComp->GetName().Contains(FString("GripperBase")))
		{
			bControllerVibrate = false;
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor = %s"), *OtherComp->GetName());
		}
	}
}

void ARobotView::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetName().Contains(FString("GripperBase")))
	{
		bControllerVibrate = true;
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor Left= %s"), *OtherComp->GetName());
	}
}

void ARobotView::PlayHapticFeedback()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayHapticEffect(mHapticFeedback, EControllerHand::Left);
}

void ARobotView::MotionControlLeftTriggerPressed()
{
	
	// Find all movable objects
	for (TActorIterator<AStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr)
		{
			AActor* Mesh = Cast<AActor>(*ActorItr);
			if (ActorItr->IsRootComponentMovable())
			{
				// If objects are inside the viewport

			// for each, generate a fake mesh without collision and physics at a random error position
				if (*ActorItr != NULL)
				{
					UWorld* const World = GetWorld();
					if (World)
					{
						// Set the spawn parameters
						FActorSpawnParameters SpawnParams;
						//SpawnParams.Template = Mesh;
						SpawnParams.Owner = this;
						SpawnParams.Instigator = Instigator;
						SpawnParams.Template = Mesh;
						// The random error location to spawn  + FMath::VRand() * 5
						FVector SpawnLocation = ActorItr->GetActorLocation() + FMath::VRand() * 5;
						// The rotation to spawn
						FRotator SpawnRotation = ActorItr->GetActorRotation();
						// Spawn the objects
						ASpawnActor* SpawnActor = World->SpawnActorAbsolute<ASpawnActor>(SpawnLocation, SpawnRotation,SpawnParams);
						//SpawnActor->SetActorEnableCollision(false);
						//SpawnActor->SetActorHiddenInGame(false);
						//SpawnActor->GetStaticMeshComponent()->SetHiddenInGame(false);
						//SpawnActor->SetMobility(EComponentMobility::Movable);
						//SpawnActor->GetStaticMeshComponent()->SetStaticMesh(ActorItr->GetStaticMeshComponent()->GetStaticMesh());
						//UE_LOG(LogTemp, Warning, TEXT("SpawnActor name= %s"),*SpawnActor->GetStaticMeshComponent()->GetName());
						if (SpawnActor)
						{
							UE_LOG(LogTemp, Warning, TEXT("SpawnActor name= %s"), *SpawnActor->GetName());
							UE_LOG(LogTemp, Warning, TEXT("SpawnActor location= %s"), *SpawnActor->GetActorLocation().ToString());
						}


					}
				}
			}
		}
		
		
		
	}
	

	
	
	

}





