// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPawn.h"
#include "XRMotionControllerBase.h"


// Sets default values
ATestPawn::ATestPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Create Root Component
	MCRoot = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = MCRoot;

	// Create the left motion controller
	MCLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MCLeft"));
	MCLeft->MotionSource = FXRMotionControllerBase::LeftHandSourceId;
	MCLeft->SetupAttachment(MCRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(MCRoot);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	//TriggerBox->bGenerateOverlapEvents = true;
	//Register Events
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATestPawn::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ATestPawn::OnOverlapEnd);

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

void ATestPawn::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UStaticMeshComponent* myComp = Cast<UStaticMeshComponent>(OtherComp);
	if ((OtherActor != nullptr) && (myComp != NULL))
	{
		
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor = %s"), *OtherComp->GetName());
	}
	
	

}

void ATestPawn::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor Left = %s"), *OtherComp->GetName());
}

