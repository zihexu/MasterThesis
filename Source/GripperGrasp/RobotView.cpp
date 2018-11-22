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
	Frustum->GetGenerateOverlapEvents();
	Frustum->SetGenerateOverlapEvents(true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>FrustumAsset(TEXT("/Game/Models/Frustum/SM_Frustum.SM_Frustum")); 
	if (FrustumAsset.Succeeded())
	{
		Frustum->SetStaticMesh(FrustumAsset.Object);
		Frustum->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
		Frustum->SetRelativeLocation(FVector(97.0f, 0.0f, 0.0f));
		Frustum->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
	}

	//Create SpringArm Component
	SpringArmComponent= CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(VRCamera);

	/*
	// Create the capsule component for body collision
	CapsuleTrigger= CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	CapsuleTrigger->SetupAttachment(SpringArmComponent);
	CapsuleTrigger->SetCollisionProfileName(TEXT("OverlapAll"));
	CapsuleTrigger->SetGenerateOverlapEvents(true);
	CapsuleTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, -60.0f));
	CapsuleTrigger->SetCapsuleRadius(42.0f);
	CapsuleTrigger->SetCapsuleHalfHeight(90.f);
	static ConstructorHelpers::FObjectFinder<UMaterial>BoundaryMaterial(TEXT("/Game/PostFX/LocationBasedOpacity/M_Boundary.M_Boundary"));
	auto* MaterialInstance = UMaterialInstanceDynamic::Create(BoundaryMaterial.Object, BoundaryMaterial.Object);
	CapsuleMaterial = CreateDefaultSubobject<UMaterial>(TEXT("CapsuleMaterial"));
	CapsuleTrigger->SetMaterial(0, MaterialInstance);

	//Register Events
	//CapsuleTrigger->OnComponentBeginOverlap.AddDynamic(this, &ARobotView::OnCapsuleOverlapBegin);
	//CapsuleTrigger->OnComponentEndOverlap.AddDynamic(this, &ARobotView::OnCapsuleOverlapEnd);
	*/
	
	//Set up the default value
	OverlapNum = 0;

	CapsuleTriggerVC= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CapsuleTriggerVC"));
	CapsuleTriggerVC->SetupAttachment(SpringArmComponent);
	CapsuleTriggerVC->SetHiddenInGame(true);
	CapsuleTriggerVC->SetCollisionProfileName(TEXT("OverlapAll"));
	CapsuleTriggerVC->SetGenerateOverlapEvents(true);
	//Register Events
	CapsuleTriggerVC->OnComponentBeginOverlap.AddDynamic(this, &ARobotView::OnCapsuleOverlapBegin);
	CapsuleTriggerVC->OnComponentEndOverlap.AddDynamic(this, &ARobotView::OnCapsuleOverlapEnd);

	// Create the left motion controller
	MCLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCLeft"));
	MCLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MCLeft->SetupAttachment(MCRoot);

	// Create the right motion controller
	MCRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCRight"));
	MCRight->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	MCRight->SetupAttachment(MCRoot);

	//LeftSphere for left controller overlap tracking
	LeftSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftSphere"));
	LeftSphere->SetupAttachment(VRCamera);
	LeftSphere->SetCollisionProfileName(TEXT("OverlapAll"));
	//LeftSphere->bGenerateOverlapEvents = true;
	//Register Events
	LeftSphere->OnComponentBeginOverlap.AddDynamic(this, &ARobotView::OnOverlapBegin);
	LeftSphere->OnComponentEndOverlap.AddDynamic(this, &ARobotView::OnOverlapEnd);

	
	// LeftSphereVisualCuesComponent
	LeftSphereVC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftSphereVC"));
	LeftSphereVC->SetupAttachment(VRCamera);
	LeftSphereVC->SetCollisionProfileName(TEXT("OverlapAll"));
	static ConstructorHelpers::FObjectFinder<UMaterial>LeftSphereVCMaterial(TEXT("/Game/PostFX/LocationBasedOpacity/M_LeftVisualMaterial.M_LeftVisualMaterial"));
	LeftSphereVC->SetMaterial(0,(UMaterial*)LeftSphereVCMaterial.Object);
	LeftSphereVC->SetRelativeLocation(FVector(0.0f,-20.0f,-25.0f));
	
	// LeftSphereVisualCuesComponent
	RightSphereVC = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightSphereVC"));
	RightSphereVC->SetupAttachment(VRCamera);
	RightSphereVC->SetCollisionProfileName(TEXT("OverlapAll"));
	static ConstructorHelpers::FObjectFinder<UMaterial>RightSphereVCMaterial(TEXT("/Game/PostFX/LocationBasedOpacity/M_RightVisualMaterial.M_RightVisualMaterial"));
	RightSphereVC->SetMaterial(0, (UMaterial*)RightSphereVCMaterial.Object);
	RightSphereVC->SetRelativeLocation(FVector(0.0f, 20.0f, -25.0f));

	
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
		AStaticMeshActor *Mesh = *ActorItr;
		if (Mesh->IsRootComponentMovable())
		{
			
			Mesh->SetActorHiddenInGame(true);
		
			//ActorItr->GetStaticMeshComponent()->SetHiddenInGame(true);
			
		}
			
	}


	
}

// Called every frame
void ARobotView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Check if motion controller is outside of range
	if (bControllerVibrate)
	{
		//UE_LOG(LogTemp, Warning, TEXT("start vibrate"));
		
	}
	
	APlayerController * MyPc = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPc)
	{
		//MyPc->ClientPlayForceFeedback(ForceFeedbackEffect, false, false, NAME_None);
		PlayHapticFeedback();
	}
	
	
	//Do the camera range detection every tick
	
}

// Called to bind functionality to input
void ARobotView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Motion Controller input binding
	InputComponent->BindAction("LeftTriggerPressed", EInputEvent::IE_Pressed, this, &ARobotView::MotionControlLeftTriggerPressed);

}

void ARobotView::OnCapsuleOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->IsRootComponentStatic())
		{
			bCameraOutside = true;
			OtherComp->SetRenderCustomDepth(true);
			OverlapNum = OverlapNum + 1;
			CapsuleTriggerVC->SetHiddenInGame(false);
			GetWorldTimerManager().SetTimer(TimerHandle,this,&ARobotView::CameraRangeDetection, 0.0f, false);
			UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor = %s"), *OtherActor->GetName());
			UE_LOG(LogTemp, Warning, TEXT("overlapping Actor number %d"), OverlapNum);
		}
	}
}

void ARobotView::OnCapsuleOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->IsRootComponentStatic())
		{
			OverlapNum = OverlapNum - 1;
			if (OverlapNum == 0)
			{
				
				bCameraOutside = false;
				OtherComp->SetRenderCustomDepth(false);
				CameraRangeDetection();
				CapsuleTriggerVC->SetHiddenInGame(true);
				UE_LOG(LogTemp, Warning, TEXT("Nothing is overlapping"));
			}
		}
	}
}

void ARobotView::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != nullptr)
	{
		if (OtherComp->GetName().Contains(FString("GripperBase")))
		{
			bControllerVibrate = false;
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor = %s"), *OtherComp->GetName());
		}
	}
}

void ARobotView::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->GetName().Contains(FString("GripperBase")))
	{
		bControllerVibrate = true;
		//UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor Left= %s"), *OtherComp->GetName());
	}
}

void ARobotView::PlayHapticFeedback()
{
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayHapticEffect(mHapticFeedback, EControllerHand::Left);
}

// Render the fake meshes with offset after left trigger pressed
void ARobotView::MotionControlLeftTriggerPressed()
{
	// Destroy cloned objects
	for (int32 Index = 0; Index != ClonedObjects.Num(); ++Index)
	{
	
		ClonedObjects[Index]->Destroy();
		//UE_LOG(LogTemp, Warning, TEXT("destroyed actor %s"), *ClonedObjects[Index]->GetName());
		
	}
	ClonedObjects.Empty();

	// Find all movable objects
	UWorld* const World = GetWorld();
	for (TActorIterator<AStaticMeshActor> ActorItr(World); ActorItr; ++ActorItr)
	{


		if (ActorItr->IsRootComponentMovable())
		{
			AStaticMeshActor* Mesh = *ActorItr;
			//UE_LOG(LogTemp, Warning, TEXT("find a actor %s"), *Mesh->GetName());
			UpdateMeshes.Add(Mesh);		
		}
	}
	// Spawn the template actors
	for (int32 Index = 0; Index != UpdateMeshes.Num(); ++Index)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AStaticMeshActor* DuplicateMesh = UpdateMeshes[Index];
		SpawnParams.Template = DuplicateMesh;

		// The random error location to spawn  + FMath::VRand() * 5
		FVector SpawnLocation = FMath::VRand() * 1;
		
		// The rotation to spawn
		FRotator SpawnRotation = FRotator(0.0f,0.0f,0.0f);
		
		// Spawn the objects 
		AStaticMeshActor* SpawnActor = World->SpawnActor<AStaticMeshActor>(DuplicateMesh->GetClass(), SpawnLocation, SpawnRotation, SpawnParams);
		SpawnActor->SetActorHiddenInGame(false);
		SpawnActor->DisableComponentsSimulatePhysics();
		SpawnActor->GetStaticMeshComponent()->SetCollisionProfileName(TEXT("OverlapAll"));
		ClonedObjects.Add(SpawnActor);
		//UE_LOG(LogTemp, Warning, TEXT("location %s"), *DuplicateMesh->GetActorLocation().ToString());
		//UE_LOG(LogTemp, Warning, TEXT("spawn location %s"), *SpawnActor->GetActorLocation().ToString());
	}
	// Empty all elements in array
	UpdateMeshes.Empty();

}

void ARobotView::CameraRangeDetection()
{
	// Check if the camera is outside of range, change the post process setting, color gamma	
	if (bCameraOutside)
	{
		FPostProcessSettings PostPro;

		PostPro.bOverride_ColorGain = true;
		PostPro.ColorGain = FVector4(0.0f, 0.0f, 0.0f, 1.0f);

		VRCamera->PostProcessSettings = PostPro;
		UE_LOG(LogTemp, Warning, TEXT("Outside"));
	}
	else if (!bCameraOutside)
	{
		FPostProcessSettings PostPro;

		PostPro.bOverride_ColorGain = true;
		PostPro.ColorGain = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		VRCamera->PostProcessSettings = PostPro;
	}

	/*
	// Check if the camera is outside of range, change the post process setting, vigenette
	if (bCameraOutside)
	{
	FPostProcessSettings PostPro;

	PostPro.bOverride_VignetteIntensity= true;
	PostPro.VignetteIntensity = 5;
	VRCamera->PostProcessSettings = PostPro;
	UE_LOG(LogTemp, Warning, TEXT("Outside"));
	}
	else if (!bCameraOutside)
	{
	FPostProcessSettings PostPro;

	PostPro.bOverride_VignetteIntensity = true;
	PostPro.VignetteIntensity = 0;
	VRCamera->PostProcessSettings = PostPro;
	}
	*/
}
	

	
	
	







