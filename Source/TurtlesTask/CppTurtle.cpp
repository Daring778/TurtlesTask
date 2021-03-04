// Fill out your copyright notice in the Description page of Project Settings.


#include "CppTurtle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACppTurtle::ACppTurtle()
	:
	CollisionScale(70.0f, 40.0f, 60.0f),
	Speed(120.f),
	bMoveForward(true),
	bWaitToMove(false),
	FinishEffectScale(1.f),
	DelayStop(0.5f),
	DelayForwardMove(1.f),
	DelayBackwardMove(0.5f),
	bPlayFinishSound(true),
	bFinish(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneC"));
	SetRootComponent(SceneComp);

	TurtleCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Turtle_Collision"));
	TurtleCollision->SetupAttachment(SceneComp);
	TurtleCollision->InitBoxExtent(CollisionScale);
	TurtleCollision->SetRelativeLocation(FVector(0.f, 0.f, 60.f));

	TurtleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turtle_Mesh"));
	TurtleMesh->SetupAttachment(TurtleCollision);
	TurtleMesh->SetWorldScale3D(FVector(0.01f));
	TurtleMesh->SetWorldRotation(FRotator(0.f, -90.f, 0.f));

	FinishAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Turtle_AudioComp"));
	FinishAudioComp->SetupAttachment(SceneComp);
	FinishAudioComp->bAutoActivate = false;

}

// Called when the game starts or when spawned
void ACppTurtle::BeginPlay()
{
	Super::BeginPlay();	

	if(FinishCueSound->IsValidLowLevelFast())
	FinishAudioComp->SetSound(FinishCueSound); // add cue in audio component

	StartRace();	
}

// Called every frame
void ACppTurtle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	


	if (!bWaitToMove) // start move functions
	{
		if (bMoveForward) // direction move condition
			MoveForward();
		else
			MoveBackward();
	}
		
}


void ACppTurtle::StartRace()
{
	switch (MoveType) // select logic according MoveType
	{
	case TypeMoving::SimpeMoving:
		bWaitToMove = false;
		bMoveForward = true;
		UE_LOG(LogTemp, Log, TEXT("SimpleMoving!"));
		break;
	case TypeMoving::MoveWithPause:
		bWaitToMove = false;
		bMoveForward = true;
		GetWorld()->GetTimerManager().SetTimer(TimerMoving, this, &ACppTurtle::WaitMove, DelayForwardMove, false);
		UE_LOG(LogTemp, Log, TEXT("MoveWithPause!"));
		break;
	case TypeMoving::MoveComplex:
		bWaitToMove = false;
		bMoveForward = true;
		GetWorld()->GetTimerManager().SetTimer(TimerMoving, this, &ACppTurtle::WaitMove, DelayForwardMove, false);
		UE_LOG(LogTemp, Log, TEXT("MoveComplex!"));
		break;
	};

}



void ACppTurtle::MoveForward()
{
	
	CurrentPosition = GetActorLocation();
	CurrentPosition += GetActorForwardVector() * Speed * GetWorld()->DeltaTimeSeconds; // change current position with speed * deltaSec for geting rid of the fps dependency

	float const Distance = FVector::Dist(CurrentPosition, EndPosition); // count Distance between Current position and Finish position

	if(!(Distance<=1))
		SetActorLocation(CurrentPosition); // change actor position
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerStopMove); // stop timer after finishing
		GetWorld()->GetTimerManager().ClearTimer(TimerMoving);

		bWaitToMove = true;
		bFinish = true;

		TurtleFinishEvent.Broadcast(); // call dispatcher

		if (FinishEffect != NULL)
		{
			if (bPlayFinishSound)
			{
				FinishAudioComp->Play(); // play sound
			}			

			FinishEffectTransform.SetLocation(GetActorLocation());
			FinishEffectTransform.SetScale3D(FinishEffectScale);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FinishEffect, FinishEffectTransform, true, EPSCPoolMethod::None, true); // spawn particles
		}

		UE_LOG(LogTemp, Log, TEXT("Finish!"));
	}
		
}

void ACppTurtle::MoveBackward()
{
	CurrentPosition = GetActorLocation();
	CurrentPosition += -(GetActorForwardVector()) * Speed * GetWorld()->DeltaTimeSeconds;

	SetActorLocation(CurrentPosition);
}


void ACppTurtle::WaitMove()
{
	if (MoveType == TypeMoving::MoveComplex) // ( different Moving types) contition for move timers
	{
		UE_LOG(LogTemp, Log, TEXT("MoveBack!"));
		bMoveForward = false;
		GetWorld()->GetTimerManager().SetTimer(TimerStopMove, this, &ACppTurtle::StartMoving, DelayBackwardMove, false);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Wait!"));
		bWaitToMove = true;
		GetWorld()->GetTimerManager().SetTimer(TimerStopMove, this, &ACppTurtle::StartMoving, DelayStop, false);
	}

}

void ACppTurtle::StartMoving()
{
	UE_LOG(LogTemp, Log, TEXT("MOVE!"));
	bWaitToMove = false;
	bMoveForward = true;
	GetWorld()->GetTimerManager().SetTimer(TimerMoving, this, &ACppTurtle::WaitMove, DelayForwardMove, false);
}